#include "RESTfulpp/Client.h"
#include "RESTfulpp/Request.h"
#include <iostream>
#include <vector>

int main() {
  RESTfulpp::Client client;
  RESTfulpp::Request req("GET", "http://localhost:8080/", std::vector<char>());
  auto res = client.send(req);
  if (res.error.empty())
    std::cout << res.serialize();

  std::cout << res.error;
}
