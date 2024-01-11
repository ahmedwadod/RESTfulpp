#include "RESTfulpp/Response.h"
#include <sstream>
#include <string>
#include <unordered_map>

using namespace RESTfulpp;

std::string getHttpStatusString(unsigned int statusCode);

Response::Response() : status_code(0), version_major(0), version_minor(0) {}

Response::Response(unsigned int statusCode, std::string body)
    : status_code(statusCode), version_major(1), version_minor(1) {
  content = std::vector<char>(body.begin(), body.end());
}

std::string Response::serialize() {
  std::stringstream s;

  s << "HTTP/" << version_major << "." << version_minor << " " << status_code
    << " " << getHttpStatusString(status_code) << "\r\n";

  for (auto header : headers) {
    s << header.first << ": " << header.second << "\r\n";
  }

  s << "\r\n" << std::string(content.data(), content.size());

  return s.str();
}

std::string getHttpStatusString(unsigned int statusCode) {
  // Map of common HTTP status codes and their corresponding string literals
  static const std::unordered_map<unsigned int, std::string> statusCodeMap = {
      {100, "Continue"},
      {101, "Switching Protocols"},
      {200, "OK"},
      {201, "Created"},
      {202, "Accepted"},
      {204, "No Content"},
      {301, "Moved Permanently"},
      {302, "Found"},
      {304, "Not Modified"},
      {400, "Bad Request"},
      {401, "Unauthorized"},
      {403, "Forbidden"},
      {404, "Not Found"},
      {405, "Method Not Allowed"},
      {500, "Internal Server Error"},
      {503, "Service Unavailable"},
  };

  // Find the string literal for the given status code
  auto iter = statusCodeMap.find(statusCode);

  if (iter != statusCodeMap.end()) {
    // Return the string literal if found
    return iter->second;
  } else {
    // Return a generic "Unknown Status Code" message if not found
    return "Unknown Status Code";
  }
}
