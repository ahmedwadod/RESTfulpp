#include "RESTfulpp/Request.h"
#include "RESTfulpp/Response.h"
#include "RESTfulpp/Server.h"
#include "RESTfulpp/Logging.h"
#include "nlohmann/json_fwd.hpp"
#include <map>
#include <string>
#include <vector>

using namespace nlohmann;
int main()
{
  RESTfulpp::setLogLevel(RESTfulpp::LogLevel::DEBUG);
  RESTfulpp::Server server();
  std::map<std::string, unsigned int> people;

  server.get("/", [&people](RESTfulpp::Request req)
             {
    std::vector<json> all_people;
    for (auto key : people) {
      json person;
      person["name"] = key.first;
      person["age"] = key.second;
      all_people.emplace_back(person);
    }
    json data = all_people;

    return RESTfulpp::Response::json(200, data); });

  server.post("/", [&people](RESTfulpp::Request req)
              {
    json data = req.body_as_json();
    if (data.count("name") == 0) {
      return RESTfulpp::Response::json(400,
                                       R"({"error": "No name suplied"})"_json);
    } else if (data.count("age") == 0) {
      return RESTfulpp::Response::json(400,
                                       R"({"error": "No age supplied"})"_json);
    } else if (people.count(data["name"]) > 0) {
      return RESTfulpp::Response::json(
          400,
          R"({"error": "Person already exists on the list. Try using put to update."})"_json);
    }

    people[data["name"]] = data["age"];

    return RESTfulpp::Response::json(200, data); });

  server.put("/", [&people](RESTfulpp::Request req)
             {
    json data = req.body_as_json();
    if (data.count("name") == 0) {
      return RESTfulpp::Response::json(400,
                                       R"({"error": "No name suplied"})"_json);
    } else if (data.count("age") == 0) {
      return RESTfulpp::Response::json(400,
                                       R"({"error": "No age supplied"})"_json);
    } else if (people.count(data["name"]) == 0) {
      return RESTfulpp::Response::json(
          400,
          R"({"error": "Person does not exists. Try using post to insert."})"_json);
    }

    people[data["name"]] = data["age"];

    return RESTfulpp::Response::json(200, data); });

  server.start();
}
