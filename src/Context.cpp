#include "RESTfulpp/Internals/Context.h"
#include "RESTfulpp/Internals/Parser.h"

using namespace RESTfulpp::Internals;

ConnectionContext::ConnectionContext(ServerContext *ctx) : server_context(ctx) {
  request_parser = RequestParser();
}
