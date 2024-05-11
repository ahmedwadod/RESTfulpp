#ifndef _RESTFULPP_CONNECTION_CONTEXT_H_
#define _RESTFULPP_CONNECTION_CONTEXT_H_
#include "RESTfulpp/Internals/Router.h"

namespace RESTfulpp::Internals {
class ServerContext {
public:
  ServerContext(size_t max_size, std::vector<Router::RouteDefinition> *routes)
      : max_request_size(max_size), route_definitions(routes) {}

  size_t max_request_size;
  std::vector<Router::RouteDefinition> *route_definitions;
};

class ConnectionContext {
public:
  ConnectionContext(ServerContext* ctx) : server_context(ctx), keep_alive(false)  {}

    ServerContext *server_context;
    std::string client_address;
    bool keep_alive;
};


} // namespace RESTfulpp::Internals

#endif // !_RESTFULPP_CONNECTION_CONTEXT_H_
