#include "RESTfulpp/Server.h"
#include "RESTfulpp/Internals/Context.h"
#include "RESTfulpp/Internals/Parser.h"
#include "RESTfulpp/Internals/Router.h"
#include "RESTfulpp/Logging.h"
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

Server::Server(unsigned int max_request_length)
    : _max_req_size(max_request_length) {

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
  event_base_free(_base);
}

void Server::start(int port, std::string address) {
  log_d("Starting the server");

  // Convert address & port to sockaddr
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  if (inet_pton(AF_INET, address.c_str(), &(sin.sin_addr)) <= 0) {
    log_e("Invalid address format");
    throw "Invalid address format";
  }
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);

  auto server_ctx =
      new Internals::ServerContext(_max_req_size, &_route_definitions);
  _listener = evconnlistener_new_bind(_base, _accept_conn_cb,
                                      (void *)server_ctx, LEV_OPT_CLOSE_ON_FREE,
                                      -1, (sockaddr *)&sin, sizeof(sin));
  if (!_listener) {
    log_e("Error creating listener");
    throw "Error creating listener";
  }
  evconnlistener_set_error_cb(
      _listener, [](evconnlistener *listener, void *ctx) {
        struct event_base *base = evconnlistener_get_base(listener);
        int err = EVUTIL_SOCKET_ERROR();
        log_e("Error " + std::to_string(err) + " (" +
              evutil_socket_error_to_string(err) + ") on listener");
      });

  log_i("Server started on " + address + ":" + std::to_string(port));
  event_base_loop(_base, EVLOOP_NO_EXIT_ON_EMPTY);
}

void Server::_accept_conn_cb(evconnlistener *listener, evutil_socket_t fd,
                             sockaddr *address, int socklen, void *ctx) {
  log_d("Accepting connection");
  Internals::ServerContext *server_ctx = (Internals::ServerContext *)ctx;
  Internals::ConnectionContext *conn_ctx =
      new Internals::ConnectionContext(server_ctx);

  char addr_str[INET_ADDRSTRLEN];
  const sockaddr_in *addr_in = reinterpret_cast<const sockaddr_in *>(address);
  inet_ntop(AF_INET, &(addr_in->sin_addr), addr_str, INET_ADDRSTRLEN);
  conn_ctx->client_address = std::string(addr_str);
  log_d("Connection from " + conn_ctx->client_address);

  auto base = evconnlistener_get_base(listener);
  auto bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
  if (!bev) {
    log_e("Error creating bufferevent");
    throw "Error creating bufferevent";
  }

  conn_ctx->request_parser.set_on_complete_cb(
      [](void *args) {
        Internals::ConnectionContext *conn_ctx =
            (Internals::ConnectionContext *)args;
        auto req = conn_ctx->request_parser.snapshot();
        log_d(req.serialize());
      },
      (void *)conn_ctx);

  bufferevent_setcb(
      bev,
      (bufferevent_data_cb)[](struct bufferevent * bev, void *ctx) {
        Internals::ConnectionContext *conn_ctx =
            (Internals::ConnectionContext *)ctx;
        struct evbuffer *input = bufferevent_get_input(bev);
        size_t len = evbuffer_get_length(input);
        if (len == 0) {
          return;
        } else if (len > conn_ctx->server_context->max_request_size) {
          log_e(conn_ctx->client_address + ": Request too large");
          delete conn_ctx;
          bufferevent_free(bev);
          return;
        }
        char *data = (char *)malloc(len + 1);
        evbuffer_remove(input, data, len);
        try {
          conn_ctx->request_parser.parse(data, len);
        } catch (const char *e) {
          log_e(e);
        }
        free(data);
      },
      NULL,
      (bufferevent_event_cb)[](struct bufferevent * bev, short events,
                               void *ctx) {
        Internals::ConnectionContext *conn_ctx =
            (Internals::ConnectionContext *)ctx;
        if (events & BEV_EVENT_ERROR) {
          log_e(conn_ctx->client_address + ": Error from bufferevent");
          delete conn_ctx;
          bufferevent_free(bev);
        } else if (events & BEV_EVENT_EOF) {
          log_d(conn_ctx->client_address + ": Connection closed");
          delete conn_ctx;
          bufferevent_free(bev);
        }
      },
      (void *)conn_ctx);
  bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void Server::_route(std::string method, std::string route_template,
                    RouteHandler func) {
  auto def = Internals::Router::route_str_to_definition(route_template);
  def.method = method;
  def.handler = func;
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
