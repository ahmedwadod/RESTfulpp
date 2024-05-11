#ifndef _RESTFULPP_CONNECTION_CONTEXT_H_
#define _RESTFULPP_CONNECTION_CONTEXT_H_
#include "RESTfulpp/Internals/Parser.h"
#include "RESTfulpp/Internals/Router.h"

namespace RESTfulpp::Internals {
class ServerContext {
public:
  ServerContext() {}
  ServerContext(size_t max_size, std::vector<Router::RouteDefinition> *routes)
      : max_request_size(max_size), route_definitions(routes) {}

  size_t max_request_size;
  int keep_alive_timeout;
  // ThreadPool later
  std::vector<Router::RouteDefinition> *route_definitions;
};

class ConnectionContext {
public:
  ConnectionContext(ServerContext *ctx);

  std::string client_address;
  Internals::RequestParser request_parser;
  ServerContext *server_context;
};

} // namespace RESTfulpp::Internals

#endif // !_RESTFULPP_CONNECTION_CONTEXT_H_
