#include <catch2/catch_test_macros.hpp>
#include <regex>
#include <string>

#include "RESTfulpp/Internals/Router.h"
#include "RESTfulpp/Request.h"
#include "RESTfulpp/Response.h"
#include "RESTfulpp/Util.h"

TEST_CASE("Route to Regex: Validate", "[Router]") {
  std::string route = "/say/[text]/to/[person]";
  auto def = RESTfulpp::Internals::Router::route_str_to_definition(route);
  std::regex rr = def.route_regex;

  auto r1 = "/say/hi/to/mom";
  auto r2 = "/say/hi/to/mom/";
  auto r3 = "/say/83uu4/to/me";
  auto r4 = "/say/hi/to/";

  REQUIRE(std::regex_search(r1, rr));
  REQUIRE(std::regex_search(r2, rr));
  REQUIRE(std::regex_search(r3, rr));
  REQUIRE(!std::regex_search(r4, rr));
}

TEST_CASE("Route to Regex: Capture Groups", "[Router]") {
  std::string route = "/say/[text]/to/[person]";
  auto def = RESTfulpp::Internals::Router::route_str_to_definition(route);
  REQUIRE(def.params_names[0] == "text");
  REQUIRE(def.params_names[1] == "person");

  std::regex rr = def.route_regex;

  std::string r1 = "/say/hi/to/mom";
  std::smatch s;

  REQUIRE(std::regex_search(r1, s, rr));
  REQUIRE(s[1] == "hi");
  REQUIRE(s[2] == "mom");
}

TEST_CASE("Router: Match Simple Route", "[Router]") {
  std::string route = "/";
  auto def = RESTfulpp::Internals::Router::route_str_to_definition(route);
  std::string r1 = "/";
  std::string r2 = "/hi";

  REQUIRE(RESTfulpp::Internals::Router::match_route(def, r1).has_value());
  REQUIRE(!RESTfulpp::Internals::Router::match_route(def, r2).has_value());
}

TEST_CASE("Router: Match Complex Route", "[Router]") {
  std::string route = "/say/[text]/to/[person]";
  auto def = RESTfulpp::Internals::Router::route_str_to_definition(route);
  std::string r1 = "/say/hi/to/mom";

  auto match = RESTfulpp::Internals::Router::match_route(def, r1);
  REQUIRE(match.has_value());
  auto value = match.value();
  REQUIRE(value["text"] == "hi");
  REQUIRE(value["person"] == "mom");
}

TEST_CASE("Match Request", "[Router]") {
  std::string route = "/say/[text]/to/[person]";
  auto def = RESTfulpp::Internals::Router::route_str_to_definition(route);
  def.method = "GET";

  RESTfulpp::Request req;
  req.method = "GET";
  req.uri = RESTfulpp::Uri("/say/hi/to/mom?from=ahmed");

  auto match = RESTfulpp::Internals::Router::match_request(def, req);
  REQUIRE(match.has_value());

  auto value = match.value();
  REQUIRE(value["text"] == "hi");
  REQUIRE(value["person"] == "mom");
}

TEST_CASE("Process Request with Routes", "[Router]") {
  std::string route = "/say/[text]/to/[person]";
  auto def = RESTfulpp::Internals::Router::route_str_to_definition(route);
  def.method = "GET";
  def.handler = [](RESTfulpp::Request req) {
    return RESTfulpp::Response::plaintext(200, "Hello, World!");
  };

  std::vector<RESTfulpp::RouteDefinition> routes = {def};

  RESTfulpp::Request req1;
  req1.method = "GET";
  req1.uri = RESTfulpp::Uri("/say/hi/to/mom?from=ahmed");
  RESTfulpp::Request req2;
  req2.method = "GET";
  req2.uri = RESTfulpp::Uri("/do/something");

  auto resp1 = RESTfulpp::process_request_with_routes(
      req1, &routes);
  auto resp2 = RESTfulpp::process_request_with_routes(
      req2, &routes);
  REQUIRE(resp1.status_code == 200);
  REQUIRE(resp1.body() == "Hello, World!");
  REQUIRE(resp2.status_code == 404);
  REQUIRE(resp2.body() == "Not Found");
}
