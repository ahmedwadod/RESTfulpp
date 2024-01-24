#include <catch2/catch_test_macros.hpp>
#include <string>

#include "RESTfulpp/Parser.h"
#include "RESTfulpp/Request.h"

TEST_CASE("Request Parsing", "[Request]") {
  RESTfulpp::RequestParser p;
  std::string req_str = "GET /?say=Hi&to=Mom HTTP/2.0\r\nHost: foo.com\r\n\r\n";
  std::string post_str = "POST / HTTP/2.0\r\n"
                         "Host: foo.com\r\n"
                         "Content-Type: application/x-www-form-urlencoded\r\n"
                         "Content-Length: 13\r\n"
                         "\r\n"
                         "say=Hi&to=Mom";

  auto get_req = p.parse(req_str.c_str(), req_str.length());
  REQUIRE(get_req.uri.path == "/");
  REQUIRE(get_req.uri.query_params["say"] == "Hi");
  REQUIRE(get_req.uri.query_params["to"] == "Mom");
  REQUIRE(get_req.version_major == 2);
  REQUIRE(get_req.version_minor == 0);
  REQUIRE(get_req.method == "GET");

  auto post_req = p.parse(post_str.c_str(), post_str.length());
  REQUIRE(post_req.method == "POST");
  REQUIRE(post_req.version_major == 2);
  REQUIRE(post_req.version_minor == 0);
  REQUIRE(post_req.headers["Host"] == "foo.com");
  // REQUIRE(post_req.formData["say"] == "Hi");
  // REQUIRE(post_req.formData["to"] == "Mom");
};

TEST_CASE("Serializing Request", "[Request]") {
  RESTfulpp::RequestParser p;
  std::string post_str = "POST / HTTP/2.0\r\n"
                         "Host: foo.com\r\n"
                         "Content-Type: application/x-www-form-urlencoded\r\n"
                         "Content-Length: 13\r\n"
                         "\r\n"
                         "say=Hi&to=Mom";

  auto req = p.parse(post_str.c_str(), post_str.length());
  REQUIRE(req.serialize() == post_str);
};
