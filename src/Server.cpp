#include "RESTfulpp/Server.h"
#include "RESTfulpp/Internals/Context.h"
#include "RESTfulpp/Internals/Parser.h"
#include "RESTfulpp/Internals/Router.h"
#include "RESTfulpp/Logging.h"
#include "RESTfulpp/Request.h"
#include "RESTfulpp/Response.h"
#include "event2/event.h"
#include <arpa/inet.h>
#include <cstddef>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <vector>

using namespace RESTfulpp;

Server::Server(unsigned int max_request_length, int keep_alive_timeout)
    : _max_req_size(max_request_length),
      keep_alive_timeout(keep_alive_timeout) {

  // ================== Create the libevent event base ==================
  log_d("Creating libevent eventbase..");
  _base = event_base_new();
  if (_base == NULL) {
    log_e("Error creating libevent event_base");
    throw "Error creating libevent event_base";
  }

  // ================== Create SIGNINT event ==================
  event *signint_event = evsignal_new(
      _base, SIGINT,
      [](evutil_socket_t sig, short events, void *user_data) {
        log_d("Caught SIGINT, shutting down server");
        event_base_loopbreak((event_base *)user_data);
      },
      _base);
  if (signint_event == NULL) {
    log_e("Error creating signal event");
    throw "Error creating signal event";
  }
  evsignal_add(signint_event, NULL);
}

Server::~Server() {
  log_d("Stopping server");
  delete ctx;
  event_base_free(_base);
}

void Server::start(int port, std::string address) {
  log_d("Starting the server");
  // Create the server context
  ctx = new Internals::ServerContext(_max_req_size, &_route_definitions,
                                     keep_alive_timeout);

  // Convert address & port to sockaddr
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  if (inet_pton(AF_INET, address.c_str(), &(sin.sin_addr)) <= 0) {
    log_e("Invalid address format");
    throw "Invalid address format";
  }
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);

  // Create the listener
  _listener = evconnlistener_new_bind(_base, Server::ntwrk_accept_conn_cb,
                                      (void *)ctx, LEV_OPT_CLOSE_ON_FREE, -1,
                                      (sockaddr *)&sin, sizeof(sin));
  if (!_listener)
    log_e("Error creating listener on port " + std::to_string(port));
  evconnlistener_set_error_cb(_listener, Server::ntwrk_listener_error_cb);

  log_i("Server started on " + address + ":" + std::to_string(port));
  event_base_loop(_base, EVLOOP_NO_EXIT_ON_EMPTY);
}

void Server::_route(std::string method, std::string route_template,
                    RouteHandler func) {
  auto def = Internals::Router::route_str_to_definition(route_template);
  def.method = method;
  def.handler = func;
  _route_definitions.push_back(def);
}

void Server::_route(std::string method, std::string route_template,
                    std::vector<MiddlewareHandler> middlewares,
                    RouteHandler func) {
  auto def = Internals::Router::route_str_to_definition(route_template);
  def.method = method;
  def.handler = func;
  def.middlewares = std::vector(middlewares);
  _route_definitions.push_back(def);
}

void Server::any(std::string route_template, RouteHandler func) {
  _route("ANY", route_template, func);
}
void Server::get(std::string route_template, RouteHandler func) {
  _route("GET", route_template, func);
}
void Server::post(std::string route_template, RouteHandler func) {
  _route("POST", route_template, func);
}
void Server::put(std::string route_template, RouteHandler func) {
  _route("PUT", route_template, func);
}
void Server::Delete(std::string route_template, RouteHandler func) {
  _route("DELETE", route_template, func);
}
void Server::patch(std::string route_template, RouteHandler func) {
  _route("PATCH", route_template, func);
}

void Server::head(std::string route_template, RouteHandler func) {
  _route("HEAD", route_template, func);
}

void Server::options(std::string route_template, RouteHandler func) {
  _route("OPTIONS", route_template, func);
}

void Server::trace(std::string route_template, RouteHandler func) {
  _route("TRACE", route_template, func);
}

void Server::get(std::string route_template,
                 std::vector<MiddlewareHandler> middlewares, RouteHandler func) {
  _route("GET", route_template, middlewares, func);
}