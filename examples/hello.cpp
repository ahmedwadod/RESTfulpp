#include "RESTfulpp/Logging.h"
#include "RESTfulpp/Request.h"
#include "RESTfulpp/Response.h"
#include "RESTfulpp/Server.h"
#include <iostream>

int main() {
  RESTfulpp::setLogLevel(RESTfulpp::LogLevel::DEBUG);
  RESTfulpp::Server s(2048);

  s.any("/", [](RESTfulpp::Request req) {
    return RESTfulpp::Response::html(200, "<h1>Hello</h1>");
  });

  s.get("/say/[text]/to/[person]", [](RESTfulpp::Request req) {
    std::string html = "<h1>" + req.url_params["text"] + ", " +
                       req.url_params["person"] + "</h1>";

    if (req.uri.query_params.count("from") > 0)
      html += "<p>From " + req.uri.query_params["from"] + "</p>";

    html += "<p>IP: " + req.client_ip + "</p>";

    return RESTfulpp::Response::html(200, html);
  });

  s.get("/error", [](RESTfulpp::Request req) {
    throw std::runtime_error("This is an error");
    return RESTfulpp::Response::html(200, "<h1>Should not be here</h1>");
  });

  s.start();
  return 0;
}
