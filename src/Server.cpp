#include "RESTfulpp/Server.h"
#include "RESTfulpp/Parser.h"
#include "RESTfulpp/Request.h"
#include "RESTfulpp/Response.h"
#include "sockpp/inet_address.h"
#include "sockpp/socket.h"
#include "sockpp/tcp_acceptor.h"
#include "sockpp/tcp_socket.h"
#include <cstddef>
#include <exception>
#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <utility>
#include <vector>

using namespace RESTfulpp;

Server::Server(short port, unsigned int max_request_length)
    : _max_req_size(max_request_length) {
  sockpp::initialize();
  _acceptor = sockpp::tcp_acceptor(port);
  if (!_acceptor) {
    std::cerr << "Error listening on port " << port << "\n";
  } else {
    std::cout << "Server will listen on port " << port << "\n";
  }

  tcpClientHandler =
      [&](std::pair<sockpp::tcp_socket, sockpp::inet_address> job) {
        std::vector<char> req_buf(_max_req_size, 0);
        size_t n = job.first.read(req_buf.data(), req_buf.size());
        Request req;
        try {
          RequestParser parser;
          req = parser.parse(req_buf.data(), n);
        } catch (std::exception e) {
          std::cerr << e.what();
          Response res = Response::plaintext(400, "Bad Request");
          job.first.write(res.serialize());
          return;
        }

        // Temp
        auto tid = std::this_thread::get_id();
        std::stringstream s;
        s << tid;
        Response res =
            Response::html(200, "<h1>OK from Thread: " + s.str() + "</h1>");
        job.first.write(res.serialize());

        return;
      };

  _pool = new ThreadPool(tcpClientHandler);
}

Server::~Server() {
  _pool->stop();
  delete _pool;
}

void Server::start(int thread_count) {
  std::cout << "Server starting up...\n";
  _pool->start(thread_count);

  while (true) {
    sockpp::inet_address client_addr;
    sockpp::tcp_socket c_sock = _acceptor.accept(&client_addr);

    if (!c_sock) {
      std::cerr << "Error accepting connections\n";
      std::cerr << _acceptor.last_error_str() << "\n";
    } else {
      std::cout << "Client connected: " << client_addr.to_string() << "\n";
      _pool->queue_job(std::make_pair(std::move(c_sock), client_addr));
    }
  }
}

// ThreadPool
ThreadPool::ThreadPool(){};
ThreadPool::ThreadPool(
    std::function<void(std::pair<sockpp::tcp_socket, sockpp::inet_address>)>
        runner_function)
    : runner(runner_function) {}

void ThreadPool::start(unsigned int num_threads) {
  for (unsigned int i = 0; i < num_threads; i++) {
    threads.emplace_back(&ThreadPool::_thread_loop, this);
  }
}

void ThreadPool::queue_job(
    std::pair<sockpp::tcp_socket, sockpp::inet_address> job) {
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    jobs.push(std::move(job));
  }

  mutex_condition.notify_one();
}

void ThreadPool::_thread_loop() {
  while (1) {
    std::pair<sockpp::tcp_socket, sockpp::inet_address> job;
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      mutex_condition.wait(
          lock, [this]() { return !jobs.empty() || should_terminate; });
      if (should_terminate) {
        return;
      }
      job = std::move(jobs.front());
      jobs.pop();
    }
    runner(std::move(job));
  }
}

bool ThreadPool::is_busy() {
  bool poolbusy;
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    poolbusy = !jobs.empty();
  }
  return poolbusy;
}

void ThreadPool::stop() {
  if (should_terminate)
    return;
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    should_terminate = true;
  }
  mutex_condition.notify_all();
  for (std::thread &active_thread : threads) {
    active_thread.join();
  }
  threads.clear();
}
