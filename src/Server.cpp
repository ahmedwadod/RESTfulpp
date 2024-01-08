#include "RESTfulpp/Server.h"
#include "RESTfulpp/Request.h"
#include "httpparser/httprequestparser.h"
#include "httpparser/request.h"
#include "httpparser/response.h"
#include "httpparser/urlparser.h"
#include "sockpp/inet_address.h"
#include "sockpp/socket.h"
#include "sockpp/tcp_acceptor.h"
#include "sockpp/tcp_socket.h"
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

using namespace RESTfulpp;

Server::Server(short port, unsigned int max_request_length) {
  sockpp::initialize();
  _acceptor = sockpp::tcp_acceptor(port);
  if (!_acceptor) {
    std::cerr << "Error listening on port " << port << "\n";
  } else {
    std::cout << "Server will listen on port " << port << "\n";
  }

  tcpClientHandler = [&max_request_length](sockpp::tcp_socket sock) {
    std::vector<char> req_buf(max_request_length, 0);
    size_t n = sock.read(req_buf.data(), max_request_length - 1);

    std::cout << "Read " << n << " bytes\n";
    RESTfulpp::Request req(req_buf, n);
    if (!req.error.empty()) {
      sock.write(httpparser::Response(400).to_string());
      sock.close();
      return 1;
    } else {
      httpparser::Response resp(200);
      std::string ok = "OK!";
      resp.content = std::vector<char>(ok.begin(), ok.end());
      sock.write(resp.to_string());
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
