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

TEST_CASE("Partial header Request", "RequestParser") {
  std::string req_str1 = "GET /do/something HTTP/1.1\r\n"
                         "Host: foo.com\r\n"
                         "Content-Type:";
  std::string req_str2 = " text/plain\r\n";
  RESTfulpp::Internals::RequestParser p;
  p.process(req_str1.c_str(), req_str1.length());
  p.process(req_str2.c_str(), req_str2.length());
  auto req = p.snapshot();

  REQUIRE(req.method == "GET");
  REQUIRE(req.uri.path == "/do/something");
  REQUIRE(req.version_major == 1);
  REQUIRE(req.version_minor == 1);
  REQUIRE(req.headers["Host"] == "foo.com");
  REQUIRE(req.headers["Content-Type"] == "text/plain");
}

TEST_CASE("Partial Body Request", "[RequestParser]") {
  std::string req_str1 = "POST /do/something HTTP/1.1\r\n"
                         "Host: foo.com\r\n"
                         "Content-Length: 13\r\n"
                         "Content-Type: text/plain\r\n"
                         "\r\n"
                         "Hello,";
  std::string req_str2 = " World!";
  RESTfulpp::Internals::RequestParser p;
  p.process(req_str1.c_str(), req_str1.length());
  p.process(req_str2.c_str(), req_str2.length());
  auto req = p.snapshot();

  REQUIRE(req.method == "POST");
  REQUIRE(req.uri.path == "/do/something");
  REQUIRE(req.headers["Host"] == "foo.com");
  REQUIRE(req.version_major == 1);
  REQUIRE(req.version_minor == 1);
  REQUIRE(req.body_as_text() == "Hello, World!");
}

TEST_CASE("Sequential Requests", "[RequestParser]") {
  std::string req1_str = "GET / HTTP/1.1\r\nHost:foo.com\r\n\r\n";
  std::string req2_str = "POST /do/something HTTP/1.1\r\n"
                         "Host: foo.com\r\n"
                         "Content-Length: 13\r\n"
                         "Content-Type: text/plain\r\n"
                         "\r\n"
                         "Hello, World!";
  RESTfulpp::Internals::RequestParser p;
  p.process(req1_str.c_str(), req1_str.length());
  auto req1 = p.snapshot();
  p.process(req2_str.c_str(), req2_str.length());
  auto req2 = p.snapshot();

  REQUIRE(req1.method == "GET");
  REQUIRE(req1.uri.path == "/");
  REQUIRE(req1.headers["Host"] == "foo.com");
  REQUIRE(req1.version_major == 1);
  REQUIRE(req1.version_minor == 1);

  REQUIRE(req2.method == "POST");
  REQUIRE(req2.uri.path == "/do/something");
  REQUIRE(req2.headers["Host"] == "foo.com");
  REQUIRE(req2.version_major == 1);
  REQUIRE(req2.version_minor == 1);
  REQUIRE(req2.body_as_text() == "Hello, World!");
}

TEST_CASE("Sequential Partial Requests", "[RequestParser]") {
  std::string req1_str1 = "GET / HTTP/1.1\r\n"
                          "Host: foo.com\r\n"
                          "Content-Type:";
  std::string req1_str2 = " text/plain\r\n\r\n";

  std::string req2_str1 = "POST /do/something HTTP/1.1\r\n"
                          "Host: foo.com\r\n"
                          "Content-Length: 13\r\n"
                          "Content-Type: text/plain\r\n"
                          "\r\n"
                          "Hello,";
  std::string req2_str2 = " World!";
  RESTfulpp::Internals::RequestParser p;
  p.process(req1_str1.c_str(), req1_str1.length());
  p.process(req1_str2.c_str(), req1_str2.length());
  auto req1 = p.snapshot();
  p.process(req2_str1.c_str(), req2_str1.length());
  p.process(req2_str2.c_str(), req2_str2.length());
  auto req2 = p.snapshot();

  REQUIRE(req1.method == "GET");
  REQUIRE(req1.uri.path == "/");
  REQUIRE(req1.headers["Host"] == "foo.com");
  REQUIRE(req1.headers["Content-Type"] == "text/plain");
  REQUIRE(req1.version_major == 1);
  REQUIRE(req1.version_minor == 1);

  REQUIRE(req2.method == "POST");
  REQUIRE(req2.uri.path == "/do/something");
  REQUIRE(req2.headers["Host"] == "foo.com");
  REQUIRE(req1.headers["Content-Type"] == "text/plain");
  REQUIRE(req2.version_major == 1);
  REQUIRE(req2.version_minor == 1);
  REQUIRE(req2.body_as_text() == "Hello, World!");
}
