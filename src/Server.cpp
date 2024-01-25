#include "RESTfulpp/Server.h"
#include "RESTfulpp/Parser.h"
#include "RESTfulpp/Request.h"
#include "RESTfulpp/Response.h"
#include "sockpp/inet_address.h"
#include "sockpp/socket.h"
#include "sockpp/tcp_acceptor.h"
#include "sockpp/tcp_socket.h"
#include <cstddef>
#include <iostream>
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

  tcpClientHandler = [&](sockpp::tcp_socket sock,
                         sockpp::inet_address client_addr) {
    //    std::vector<char> req_buf(_max_req_size, 0);
    //    size_t n = sock.read(req_buf.data(), req_buf.size());
    //    RequestParser parser;
    //    auto req = parser.parse(req_buf.data(), n);
    //
    //    if (!req.error.empty()) {
    //      std::cout << req.error << "\n";
    //      Response res(400, "Bad Request");
    //      std::cout << res.serialize();
    //      sock.write(res.serialize());
    //      sock.close();
    //      return 1;
    //    } else {
    //      sock.write("HTTP/1.1 200 OK\r\n");
    //      sock.write("Content-Type: text/html\r\n\r\n");
    //      sock.write("<h1>Loading...</h1>");
    //      sleep(3);
    //      sock.write("<h2>Yaaass!</h2>");
    //    }

    return 0;
  };
}

void Server::start(int thread_count) {
  std::cout << "Server starting up...\n";

  while (true) {
    sockpp::inet_address client_addr;
    sockpp::tcp_socket c_sock = _acceptor.accept(&client_addr);

    if (!c_sock) {
      std::cerr << "Error accepting connections\n";
      std::cerr << _acceptor.last_error_str() << "\n";
    } else {
      std::cout << "Client connected: " << client_addr.to_string() << "\n";
      tcpClientHandler(std::move(c_sock), client_addr);
    }
  }
}
