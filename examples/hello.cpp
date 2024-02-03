#include "../include/RESTfulpp/Server.h"
#include "RESTfulpp/Request.h"
#include "RESTfulpp/Response.h"
#include <iostream>

int main() {
  RESTfulpp::Server s(8080);

  s.any("/", [](RESTfulpp::Request req) {
    return RESTfulpp::Response::html(200, "<h1>Hello</h1>");
  });

  s.get("/say/[text]/to/[person]", [](RESTfulpp::Request req) {
    std::string html = "<h1>" + req.url_params["text"] + ", " +
                       req.url_params["person"] + "</h1>";

    if (req.uri.query_params.count("from") > 0) {
      html += "<p>From " + req.uri.query_params["from"] + "</p>";
    }

    html += "<p>IP: " + req.client_ip + "</p>";

    return RESTfulpp::Response::html(200, html);
  });

  s.start();
  return 0;
}
