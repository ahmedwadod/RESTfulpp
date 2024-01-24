#include <catch2/benchmark/catch_benchmark_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include "RESTfulpp/Types.h"

TEST_CASE("Full URI", "[URI]") {
  RESTfulpp::Uri uri("/ahmed/say?text=hi&to=mom");
  REQUIRE(uri.path == "/ahmed/say");
  REQUIRE(uri.query_params["text"] == "hi");
  REQUIRE(uri.query_params["to"] == "mom");
}

TEST_CASE("Short URI", "[URI]") {
  RESTfulpp::Uri uri("/");
  REQUIRE(uri.path == "/");
}

TEST_CASE("Empty URI", "[URI]") {
  RESTfulpp::Uri uri("");
  REQUIRE(uri.path == "/");
}

TEST_CASE("Medium URI", "[URI]") {
  RESTfulpp::Uri uri("/?name=ahmed");
  REQUIRE(uri.path == "/");
  REQUIRE(uri.query_params["name"] == "ahmed");
}

TEST_CASE("Full URL Parsing", "[URL]") {
  RESTfulpp::Url url("https://www.elkhalifa.dev:8090/ahmed/say?text=hi&to=me");
  REQUIRE(url.protocol == "https");
  REQUIRE(url.host == "www.elkhalifa.dev");
  REQUIRE(url.port == "8090");
  REQUIRE(url.uri.path == "/ahmed/say");
  REQUIRE(url.uri.query_params["text"] == "hi");
  REQUIRE(url.uri.query_params["to"] == "me");
}

TEST_CASE("Short URL Parsing", "[URL]") {
  RESTfulpp::Url url("https://elkhalifa.dev");
  REQUIRE(url.uri.path == "/");
  REQUIRE(url.protocol == "https");
  REQUIRE(url.host == "elkhalifa.dev");
  REQUIRE(url.port == "443");
}

TEST_CASE("Medium URL Parsing", "[URL]") {
  RESTfulpp::Url url("https://elkhalifa.dev/?name=ahmed");
  REQUIRE(url.uri.path == "/");
  REQUIRE(url.protocol == "https");
  REQUIRE(url.host == "elkhalifa.dev");
  REQUIRE(url.port == "443");
  REQUIRE(url.uri.query_params["name"] == "ahmed");
}
