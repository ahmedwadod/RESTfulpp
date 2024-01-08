#include <catch2/benchmark/catch_benchmark_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include "RESTfulpp/Types.h"

TEST_CASE("URL Parsing test", "[URL]") {
  RESTfulpp::Url url("https://www.elkhalifa.dev:8090/ahmed/say?text=hi&to=me");
  REQUIRE(url.protocol == "https");
  REQUIRE(url.host == "www.elkhalifa.dev");
  REQUIRE(url.port == "8090");
  REQUIRE(url.path == "/ahmed/say");
  REQUIRE(url.query_params["text"] == "hi");
  REQUIRE(url.query_params["to"] == "me");

  /*  BENCHMARK("URL Parser") {
      return RESTfulpp::Url(
          "https://www.elkhalifa.dev:8090/ahmed/page/index/action/"
          "say?text=hi&to=me");
    };
    */
}
