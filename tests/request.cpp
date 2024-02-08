#include <catch2/catch_test_macros.hpp>
#include <string>

#include "RESTfulpp/Internals/Parser.h"
#include "RESTfulpp/Request.h"

TEST_CASE("Serializing Request", "[Request]") {
  RESTfulpp::Internals::RequestParser p;
  std::string post_str = "POST / HTTP/2.0\r\n"
                         "Host: foo.com\r\n"
                         "Content-Type: application/x-www-form-urlencoded\r\n"
                         "Content-Length: 13\r\n"
                         "\r\n"
                         "say=Hi&to=Mom";

  auto req = p.parse(post_str.c_str(), post_str.length());
  REQUIRE(req.serialize() == post_str);
};

TEST_CASE("Request Form Data", "[Request]") {
  RESTfulpp::Internals::RequestParser p;
  std::string post_str = "POST / HTTP/2.0\r\n"
                         "Host: foo.com\r\n"
                         "Content-Type: application/x-www-form-urlencoded\r\n"
                         "Content-Length: 13\r\n"
                         "\r\n"
                         "say=Hi&to=Mom";

  auto req = p.parse(post_str.c_str(), post_str.length());
  auto formData = req.body_as_form_data();
  REQUIRE(formData["say"] == "Hi");
  REQUIRE(formData["to"] == "Mom");
};

TEST_CASE("JSON Request", "[Request]") {
  RESTfulpp::Internals::RequestParser p;
  std::string post_str = "POST /json HTTP/1.1\r\n"
                         "Host: foo.com\r\n"
                         "Content-Type: application/json\r\n"
                         "Content-Length: 33\r\n"
                         "\r\n"
                         "{\"say\": \"hi\", \"to\":\"mom\"}";
  auto req = p.parse(post_str.c_str(), post_str.length());
  auto jsonData = req.body_as_json();
  REQUIRE(jsonData["say"] == "hi");
  REQUIRE(jsonData["to"] == "mom");
}
