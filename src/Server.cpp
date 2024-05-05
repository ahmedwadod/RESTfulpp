#include "RESTfulpp/Server.h"
#include "RESTfulpp/Internals/Parser.h"
#include "RESTfulpp/Internals/Router.h"
#include "RESTfulpp/Internals/ThreadPool.h"
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
    : _max_req_size(max_request_length)
{
  sockpp::initialize();
  _acceptor = sockpp::tcp_acceptor(port);
  if (!_acceptor)
  {
    std::cerr << "Error listening on port " << port << "\n";
  }
  else
  {
    std::cout << "Server will listen on port " << port << "\n";
  }

  tcpClientHandler =
      [&](std::pair<sockpp::tcp_socket, sockpp::inet_address> job)
  {
    std::vector<char> req_buf(_max_req_size, 0);
    size_t n = job.first.read(req_buf.data(), req_buf.size());
    Request req;
    try
    {
      Internals::RequestParser parser;
      req = parser.parse(req_buf.data(), n);
    }
    catch (std::exception e)
    {
      std::cerr << e.what();
      Response res = Response::plaintext(400, "Bad Request");
      job.first.write(res.serialize());
      job.first.close();
      return;
    }

    // Handle The Request
    req.client_ip = job.second.to_string();
    for (auto def : _route_definitions)
    {
      auto match = Internals::Router::match_request(def, req);
      if (match.has_value())
      {
        try
        {
          req.url_params = match.value();
          Response resp = def.handler(req);
          job.first.write(resp.serialize());
          job.first.shutdown(SHUT_WR);
          job.first.close();
          return;
        }
        catch (std::exception e)
        {
          std::cerr << e.what();
          Response res = Response::plaintext(500, "Server Internal Error");
          job.first.write(res.serialize());
          job.first.close();
          return;
        }
      }
    }

    Response res = Response::plaintext(404, "Page not found");
    job.first.write(res.serialize());
    job.first.close();
    return;
  };

  _pool = new ThreadPool(tcpClientHandler);
}

Server::~Server()
{
  _pool->stop();
  delete _pool;
}

void Server::start(int thread_count)
{
  std::cout << "Server starting up...\n";
  _pool->start(thread_count);

  while (true)
  {
    sockpp::inet_address client_addr;
    sockpp::tcp_socket c_sock = _acceptor.accept(&client_addr);

    if (!c_sock)
    {
      std::cerr << "Error accepting connections\n";
      std::cerr << _acceptor.last_error_str() << "\n";
    }
    else
    {
      _pool->queue_job(std::make_pair(std::move(c_sock), client_addr));
    }
  }
}

void Server::_route(std::string method, std::string route_template,
                    RouteHandler func)
{
  auto def = Internals::Router::route_str_to_definition(route_template);
  def.method = method;
  def.handler = func;
  _route_definitions.push_back(def);
}
void Server::any(std::string route_template, RouteHandler func)
{
  _route("ANY", route_template, func);
}
void Server::get(std::string route_template, RouteHandler func)
{
  _route("GET", route_template, func);
}
void Server::post(std::string route_template, RouteHandler func)
{
  _route("POST", route_template, func);
}
void Server::put(std::string route_template, RouteHandler func)
{
  _route("PUT", route_template, func);
}
void Server::Delete(std::string route_template, RouteHandler func)
{
  _route("DELETE", route_template, func);
}
void Server::patch(std::string route_template, RouteHandler func)
{
  _route("PATCH", route_template, func);
}

void Server::head(std::string route_template, RouteHandler func)
{
  _route("HEAD", route_template, func);
}

void Server::options(std::string route_template, RouteHandler func)
{
  _route("OPTIONS", route_template, func);
}

void Server::trace(std::string route_template, RouteHandler func)
{
  _route("TRACE", route_template, func);
}
