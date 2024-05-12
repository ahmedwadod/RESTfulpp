#ifndef __RESTFULPP_SERVER_H__
#define __RESTFULPP_SERVER_H__

#include "RESTfulpp/Internals/Context.h"
#include "RESTfulpp/Internals/Router.h"
#include "RESTfulpp/Util.h"
#include "Util.h"
#include "event2/event.h"
#include <condition_variable>
#include <event2/listener.h>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

namespace RESTfulpp {

// Server
class Server {
public:
  Server(unsigned int max_request_length = 1024 * 1024,
         int keep_alive_timeout = DEFAULT_CONN_TIMEOUT_SEC);
  ~Server();
  void start(int port = 8080, std::string address = "127.0.0.1");

  void any(std::string route_template, RouteHandler func);
  void get(std::string route_template, RouteHandler func);
  void post(std::string route_template, RouteHandler func);
  void put(std::string route_template, RouteHandler func);
  void Delete(std::string route_template, RouteHandler func);
  void patch(std::string route_template, RouteHandler func);
  void head(std::string route_template, RouteHandler func);
  void options(std::string route_template, RouteHandler func);
  void trace(std::string route_template, RouteHandler func);
  void connect(std::string route_template, RouteHandler func);

  int keep_alive_timeout;

private:
  Internals::ServerContext *ctx;
  event_base *_base;
  evconnlistener *_listener;
  unsigned int _max_req_size;
  void _route(std::string method, std::string route_template,
              RouteHandler func);
  std::vector<RESTfulpp::RouteDefinition> _route_definitions;
  Internals::ServerContext _context;

  // Callbacks
  static void ntwrk_accept_conn_cb(evconnlistener *listener, evutil_socket_t fd,
                                   sockaddr *address, int socklen, void *ctx);
  static void ntwrk_bev_read_cb(bufferevent *bev, void *ctx);
  static void ntwrk_bev_write_cb(bufferevent *bev, void *ctx);
  static void ntwrk_bev_event_cb(bufferevent *bev, short events, void *ctx);
  static void ntwrk_listener_error_cb(evconnlistener *listener, void *ctx);
  static void ntwrk_parsing_complete_cb(void *args);
};
} // namespace RESTfulpp

#endif // !__RESTFULPP_SERVER_H__
