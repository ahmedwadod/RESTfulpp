#include "RESTfulpp/Request.h"
#include "RESTfulpp/Types.h"
#include "nlohmann/json.hpp"
#include <algorithm>
#include <ios>
#include <sstream>
#include <string>
#include <vector>

using namespace RESTfulpp;

Request::Request() : version_major(1), version_minor(1) {}

Request Request::from_form_data(std::map<std::string, std::string> form_data) {
  Request req;
  req.headers["Content-Type"] = "application/x-www-form-urlencoded";

  std::stringstream s;
  if (!form_data.empty()) {
    for (auto fd : form_data) {
      s << fd.first << "=" << fd.second << "&";
    }
    s.seekp(-1, std::ios_base::end);
  }

  auto str = s.str();
  req.content = std::vector<char>(str.begin(), str.end());

  return req;
}

std::map<std::string, std::string> Request::body_as_form_data() const {
  auto ct = headers.find("Content-Type");
  if (ct == headers.end() || ct->second != "application/x-www-form-urlencoded")
    throw "Request Header doesn't indicate it has Form Data";

  return parseParams(std::string(content.begin(), content.end()));
}

std::string Request::body_as_text() const {
  return std::string(content.begin(), content.end());
}

nlohmann::json Request::body_as_json() const {
  return nlohmann::json::parse(content.begin(), content.end());
}

std::string Request::serialize() const {
  std::stringstream s;
  s << method << " " << uri.to_string() << " ";
  s << "HTTP/" << version_major << "." << version_minor << "\r\n";

  std::for_each(headers.rbegin(), headers.rend(), [&s](auto param) {
    s << param.first << ": " << param.second << "\r\n";
  });

  s << "\r\n" << std::string(content.begin(), content.end());

  return s.str();
}
