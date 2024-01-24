#include <catch2/catch_test_macros.hpp>
#include <string>

#include "RESTfulpp/Parser.h"
#include "RESTfulpp/Request.h"

TEST_CASE("GET Request Parsing", "[Request]") {
  RESTfulpp::RequestParser p;
  std::string req_str = "GET /?say=Hi&to=Mom HTTP/2.0\r\nHost: foo.com\r\n\r\n";

  auto req = p.parse(req_str.c_str(), req_str.length());
  REQUIRE(req.uri.path == "/");
  REQUIRE(req.uri.query_params["say"] == "Hi");
  REQUIRE(req.uri.query_params["to"] == "Mom");
  REQUIRE(req.version_major == 2);
  REQUIRE(req.version_minor == 0);
  REQUIRE(req.method == "GET");
};

TEST_CASE("POST Request Parsing", "[Request]") {
  RESTfulpp::RequestParser p;
  std::string req_str = "POST / HTTP/2.0\r\n"
                        "Host: foo.com\r\n"
                        "Content-Type: application/x-www-form-urlencoded\r\n"
                        "Content-Length: 13\r\n"
                        "\r\n"
                        "say=Hi&to=Mom";

  auto req = p.parse(req_str.c_str(), req_str.length());
  REQUIRE(req.method == "POST");
  REQUIRE(req.version_major == 2);
  REQUIRE(req.version_minor == 0);
  REQUIRE(req.headers["Host"] == "foo.com");
  auto formData = req.get_form_data();
  REQUIRE(formData["say"] == "Hi");
  REQUIRE(formData["to"] == "Mom");
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
