#include "RESTfulpp/Internals/Context.h"
#include "RESTfulpp/Internals/Parser.h"
#include "RESTfulpp/Internals/Router.h"
#include "RESTfulpp/Logging.h"
#include "RESTfulpp/Response.h"
#include "RESTfulpp/Server.h"
#include "RESTfulpp/Util.h"
#include <arpa/inet.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <evutil.h>
#include <iostream>
#include <string>

using namespace RESTfulpp;

void Server::ntwrk_accept_conn_cb(evconnlistener *listener, evutil_socket_t fd,
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
  conn_ctx->buffer_event = bev;

  conn_ctx->request_parser.set_on_complete_cb(Server::ntwrk_parsing_complete_cb,
                                              (void *)conn_ctx);

  bufferevent_setcb(bev, Server::ntwrk_bev_read_cb, Server::ntwrk_bev_write_cb,
                    Server::ntwrk_bev_event_cb, (void *)conn_ctx);
  bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void Server::ntwrk_bev_read_cb(bufferevent *bev, void *ctx) {
  Internals::ConnectionContext *conn_ctx = (Internals::ConnectionContext *)ctx;
  evbuffer *input = bufferevent_get_input(bev);
  evbuffer *output = bufferevent_get_output(bev);
  size_t len = evbuffer_get_length(input);
  if (len == 0) {
    return;
  } else if (len > conn_ctx->server_context->max_request_size) {
    log_e(conn_ctx->client_address + ": Request too large");
    auto res_str =
        RESTfulpp::Response::plaintext(413, "Request too large").serialize();
    evbuffer_add(output, res_str.c_str(), res_str.size());
    return;
  }

  char *data = (char *)malloc(len + 1);
  evbuffer_remove(input, data, len);
  try {
    conn_ctx->request_parser.process(data, len);
  } catch (const char *e) {
    log_e(conn_ctx->client_address + ": " + e);
    auto res_str =
        RESTfulpp::Response::plaintext(400, "Error parsing HTTP request")
            .serialize();
    evbuffer_add(output, res_str.c_str(), res_str.size());
  }
  free(data);
}

void Server::ntwrk_bev_write_cb(bufferevent *bev, void *ctx) {
  Internals::ConnectionContext *conn_ctx = (Internals::ConnectionContext *)ctx;
  evbuffer *output = bufferevent_get_output(bev);
  if (evbuffer_get_length(output) == 0 && conn_ctx->should_terminate) {
    log_d(conn_ctx->client_address + ": Closing connection");
    delete conn_ctx;
    bufferevent_free(bev);
    return;
  }
}

void Server::ntwrk_bev_event_cb(bufferevent *bev, short events, void *ctx) {
  Internals::ConnectionContext *conn_ctx = (Internals::ConnectionContext *)ctx;
  if (events & BEV_EVENT_ERROR) {
    log_e(conn_ctx->client_address + ": Error from bufferevent");
    delete conn_ctx;
    bufferevent_free(bev);
  } else if (events & BEV_EVENT_EOF) {
    log_d(conn_ctx->client_address + ": Connection closed");
    delete conn_ctx;
    bufferevent_free(bev);
  }
}

void Server::ntwrk_listener_error_cb(evconnlistener *listener, void *ctx) {
  struct event_base *base = evconnlistener_get_base(listener);
  int err = EVUTIL_SOCKET_ERROR();
  log_e("Error " + std::to_string(err) + " (" +
        evutil_socket_error_to_string(err) + ") on listener");
}

void Server::ntwrk_parsing_complete_cb(void *args) {
  Internals::ConnectionContext *conn_ctx = (Internals::ConnectionContext *)args;
  log_d(conn_ctx->client_address + ": Request parsed");
  auto req = conn_ctx->request_parser.snapshot();
  evbuffer *output = bufferevent_get_output(conn_ctx->buffer_event);
  req.client_ip = conn_ctx->client_address;
  auto res = RESTfulpp::process_request_with_routes(
      req, conn_ctx->server_context->route_definitions);
  log_d(conn_ctx->client_address + ": Sending response " +
        std::to_string(res.status_code));
  if (conn_ctx->server_context->keep_alive_timeout > 0 &&
      req.is_request_keep_alive()) {
    res.set_response_keep_alive(true,
                                conn_ctx->server_context->keep_alive_timeout);
    conn_ctx->should_terminate = false;
    log_d(conn_ctx->client_address + ": Keep-alive for " +
          std::to_string(conn_ctx->server_context->keep_alive_timeout) +
          " seconds");
  } else {
    res.set_response_keep_alive(false, 0);
    conn_ctx->should_terminate = true;
    log_d(conn_ctx->client_address + ": Connection will close afterwards");
  }
  auto res_str = res.serialize();
  evbuffer_add(output, res_str.c_str(), res_str.size());
}