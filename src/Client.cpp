#include "../include/RESTfulpp/Client.h"
#include "RESTfulpp/Parser.h"
#include "RESTfulpp/Request.h"
#include "RESTfulpp/Response.h"
#include "sockpp/socket.h"
#include "sockpp/tcp_connector.h"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

using namespace RESTfulpp;

Client::Client() { sockpp::initialize(); }

Response Client::send(Request req) {
  if (!req.url.is_valid()) {
    return Response::ErrorResponse("Invalid Url");
  }

  unsigned short port = 80;
  if (!req.url.port.empty()) {
    port = (unsigned short)std::stoi(req.url.port);
  }

  _connector =
      sockpp::tcp_connector({req.url.host, port}, std::chrono::seconds{5});

  if (!_connector) {
    return Response::ErrorResponse(_connector.last_error_str());
  }

  auto req_data = req.serialize();
  auto wn = _connector.write(req_data.c_str(), req_data.size());
  if (wn < req_data.size()) {
    return Response::ErrorResponse("Write to socket error");
  }

  std::vector<char> res_buf(4048);
  _connector.read_n(res_buf.data(), 4048);
  ResponseParser parser;
  return parser.parse(res_buf);
}
