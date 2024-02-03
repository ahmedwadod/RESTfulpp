#include <catch2/catch_test_macros.hpp>
#include <regex>
#include <string>

#include "RESTfulpp/Request.h"
#include "RESTfulpp/Router.h"
#include "RESTfulpp/Types.h"

TEST_CASE("Route to Regex: Validate", "[Router]") {
  std::string route = "/say/[text]/to/[person]";
  auto def = RESTfulpp::Router::route_str_to_definition(route);
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
  auto def = RESTfulpp::Router::route_str_to_definition(route);
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
  auto def = RESTfulpp::Router::route_str_to_definition(route);
  std::string r1 = "/";
  std::string r2 = "/hi";

  REQUIRE(RESTfulpp::Router::match_route(def, r1).has_value());
  REQUIRE(!RESTfulpp::Router::match_route(def, r2).has_value());
}

TEST_CASE("Router: Match Complex Route", "[Router]") {
  std::string route = "/say/[text]/to/[person]";
  auto def = RESTfulpp::Router::route_str_to_definition(route);
  std::string r1 = "/say/hi/to/mom";

  auto match = RESTfulpp::Router::match_route(def, r1);
  REQUIRE(match.has_value());
  auto value = match.value();
  REQUIRE(value["text"] == "hi");
  REQUIRE(value["person"] == "mom");
}

TEST_CASE("Match Request", "[Router]") {
  std::string route = "/say/[text]/to/[person]";
  auto def = RESTfulpp::Router::route_str_to_definition(route);
  def.method = "GET";

  RESTfulpp::Request req;
  req.method = "GET";
  req.uri = RESTfulpp::Uri("/say/hi/to/mom?from=ahmed");

  auto match = RESTfulpp::Router::match_request(def, req);
  REQUIRE(match.has_value());

  auto value = match.value();
  REQUIRE(value["text"] == "hi");
  REQUIRE(value["person"] == "mom");
}
