#include "RESTfulpp/Response.h"
#include <sstream>
#include <string>
#include <unordered_map>

using namespace RESTfulpp;

std::string getHttpStatusString(unsigned int statusCode);

Response::Response() : status_code(0), version_major(1), version_minor(1) {
  headers["Server"] = "RESTfulpp";
}

Response Response::plaintext(unsigned int status_code, std::string text) {
  Response resp;
  resp.status_code = status_code;
  resp.headers["Content-Type"] = "text/plain";
  resp.content = std::vector<char>(text.begin(), text.end());
  return resp;
}

Response Response::html(unsigned int status_code, std::string text) {
  Response resp;
  resp.status_code = status_code;
  resp.headers["Content-Type"] = "text/html";
  resp.content = std::vector<char>(text.begin(), text.end());
  return resp;
}

Response Response::json(unsigned int status_code, nlohmann::json json_data) {
  Response resp;
  resp.status_code = status_code;
  resp.headers["Content-Type"] = "application/json";
  auto jdump = json_data.dump();
  resp.content = std::vector<char>(jdump.begin(), jdump.end());
  return resp;
}

std::string Response::body() const {
  return std::string(content.begin(), content.end());
}

nlohmann::json Response::body_as_json() const {
  return nlohmann::json::parse(body());
}

std::string Response::serialize() const {
  std::stringstream s;

  s << "HTTP/" << version_major << "." << version_minor << " " << status_code
    << " " << getHttpStatusString(status_code) << "\r\n";

  for (auto header : headers)
    s << header.first << ": " << header.second << "\r\n";

  s << "Content-Length: " << content.size() << "\r\n";

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

// on response keep-alive must be explicit
bool Response::is_response_keep_alive() const {
  auto connection = headers.find("Connection");
  if (connection != headers.end() && (connection->second == "keep-alive" ||
                                      connection->second == "Keep-Alive")) {
    return true;
  } else if (connection != headers.end() &&
             (connection->second == "close" || connection->second == "Close")) {
    return false;
  }

  return false;
}

void Response::set_response_keep_alive(bool keep_alive, int timeout) {
  if (keep_alive) {
    headers["Connection"] = "keep-alive";
    headers["Keep-Alive"] = "timeout=" + std::to_string(timeout);
  } else {
    headers["Connection"] = "close";
  }
}
