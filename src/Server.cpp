#include "RESTfulpp/Server.h"
#include "RESTfulpp/Request.h"
#include "RESTfulpp/Response.h"
#include "sockpp/inet_address.h"
#include "sockpp/socket.h"
#include "sockpp/tcp_acceptor.h"
#include "sockpp/tcp_socket.h"
#include <cstddef>
#include <iostream>
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

  tcpClientHandler = [&](sockpp::tcp_socket sock) {
    std::vector<char> req_buf(_max_req_size, 0);
    size_t n = sock.read(req_buf.data(), _max_req_size - 1);

    if (true) {
      sock.write("X");
      sock.close();
      return 1;
    } else {
      sock.write("G");
    }

    return 0;
  };
}

void Server::start() {
  std::cout << "Server starting up...\n";

  while (true) {
    sockpp::inet_address client_addr;
    sockpp::tcp_socket c_sock = _acceptor.accept(&client_addr);

    if (!c_sock) {
      std::cerr << "Error accepting connections\n";
      std::cerr << _acceptor.last_error_str() << "\n";
    } else {
      std::cout << "Client connected: " << client_addr.to_string() << "\n";
      tcpClientHandler(std::move(c_sock));
    }
  }
}
