#include "../include/RESTfulpp/Client.h"
#include "RESTfulpp/Request.h"
#include "RESTfulpp/Response.h"
#include "sockpp/socket.h"

using namespace RESTfulpp;

Client::Client() { sockpp::initialize(); }

Response Client::send(Request req) { return Response(); }
