#include "RESTfulpp/Logging.h"
#include "RESTfulpp/Request.h"
#include "RESTfulpp/Response.h"
#include "RESTfulpp/Server.h"
#include "RESTfulpp/Util.h"

using namespace RESTfulpp;

Response middleware1(Request &req, RouteHandler next) {
  req.headers["X-Middleware1"] = "true";
  return next(req);
}

Response middleware2(Request &req, RouteHandler next) {
  req.headers["X-Middleware2"] = "true";
  return next(req);
}

int main() {
  Server server;
  setLogLevel(LogLevel::DEBUG);

  server.get("/", {middleware1, middleware2}, [](Request &req) {
    bool mw1 = req.headers["X-Middleware1"] == "true";
    bool mw2 = req.headers["X-Middleware2"] == "true";
    nlohmann::json response = {{"middleware1", mw1}, {"middleware2", mw2}};
    return Response::json(200, response);
  });

  server.start(8080);
}
