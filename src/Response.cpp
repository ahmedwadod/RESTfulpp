#include "RESTfulpp/Response.h"
#include <sstream>
#include <string>

using namespace RESTfulpp;

Response::Response() : status_code(0), version_major(0), version_minor(0) {}

Response::Response(unsigned int statusCode, std::string body)
    : status_code(statusCode) {
  content = std::vector<char>(body.begin(), body.end());
}

std::string Response::serialize() {
  std::stringstream s;

  s << "HTTP/" << version_major << "." << version_minor << " " << status_code
    << "STATUS\r\n";

  for (auto header : headers) {
    s << header.first << ": " << header.second << "\r\n";
  }

  s << "\r\n" << std::string(content.data(), content.size());

  return s.str();
}
