#include "RESTfulpp/Logging.h"
#include "RESTfulpp/Request.h"
#include "RESTfulpp/Response.h"
#include "RESTfulpp/Server.h"
#include "RESTfulpp/Util.h"

using namespace RESTfulpp;

struct RequestData {
  bool middleware1;
  bool middleware2;
};

Response middleware1(Request &req, RouteHandler next) {
  req.set_extra<RequestData>({true, false});
  return next(req);
}

Response middleware2(Request &req, RouteHandler next) {
  req.get_extra<RequestData>()->middleware2 = true;
  return next(req);
}

int main() {
  Server server;
  setLogLevel(LogLevel::DEBUG);

  server.get("/", {middleware1, middleware2}, [](Request &req) {
    bool mw1 = req.get_extra<RequestData>()->middleware1;
    bool mw2 = req.get_extra<RequestData>()->middleware2;
    nlohmann::json response = {{"middleware1", mw1}, {"middleware2", mw2}};
    return Response::json(200, response);
  });

  server.start(8080);
}
