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

Response Client::send(Request req) { return Response(); }
