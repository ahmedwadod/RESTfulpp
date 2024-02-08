#include <catch2/catch_test_macros.hpp>

#include "RESTfulpp/Internals/Parser.h"
#include <string>

TEST_CASE("Basic GET Request", "[RequestParser]") {
  std::string req_str = "GET / HTTP/1.1\r\nHost:foo.com\r\n";
  RESTfulpp::Internals::RequestParser p;
  auto req = p.parse(req_str.c_str(), req_str.length());

  REQUIRE(req.method == "GET");
  REQUIRE(req.uri.path == "/");
  REQUIRE(req.headers["Host"] == "foo.com");
  REQUIRE(req.version_major == 1);
  REQUIRE(req.version_minor == 1);
}

TEST_CASE("GET Request with params", "[RequestParser]") {
  std::string req_str = "GET /?say=hi&to=mom HTTP/1.1\r\nHost:foo.com\r\n";
  RESTfulpp::Internals::RequestParser p;
  auto req = p.parse(req_str.c_str(), req_str.length());

  REQUIRE(req.method == "GET");
  REQUIRE(req.uri.path == "/");
  REQUIRE(req.headers["Host"] == "foo.com");
  REQUIRE(req.version_major == 1);
  REQUIRE(req.version_minor == 1);
  REQUIRE(req.uri.query_params["say"] == "hi");
  REQUIRE(req.uri.query_params["to"] == "mom");
}

TEST_CASE("Basic POST", "[RequestParser]") {
  std::string req_str = "POST /do/something HTTP/1.1\r\n"
                        "Host: foo.com\r\n"
                        "Content-Length: 13\r\n"
                        "Content-Type: text/plain\r\n"
                        "\r\n"
                        "Hello, World!";
  RESTfulpp::Internals::RequestParser p;
  auto req = p.parse(req_str.c_str(), req_str.length());

  REQUIRE(req.method == "POST");
  REQUIRE(req.uri.path == "/do/something");
  REQUIRE(req.headers["Host"] == "foo.com");
  REQUIRE(req.version_major == 1);
  REQUIRE(req.version_minor == 1);
  REQUIRE(req.body_as_text() == "Hello, World!");
}
