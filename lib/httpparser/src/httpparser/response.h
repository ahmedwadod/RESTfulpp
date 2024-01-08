/*
 * Copyright (C) Alex Nekipelov (alex@nekipelov.net)
 * License: MIT
 *
 * Code modified by Ahmed Elkhalifa (ahmed@elkhalifa.dev)
 */

#ifndef HTTPPARSER_RESPONSE_H
#define HTTPPARSER_RESPONSE_H

#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace httpparser {

struct Response {
  Response()
      : versionMajor(0), versionMinor(0), keepAlive(false), statusCode(0) {}

  Response(unsigned int code)
      : versionMajor(1), versionMinor(1), keepAlive(false), statusCode(code) {}

  struct HeaderItem {
    std::string name;
    std::string value;
  };

  int versionMajor;
  int versionMinor;
  std::vector<HeaderItem> headers;
  std::vector<char> content;
  bool keepAlive;

  unsigned int statusCode;
  std::string status;

  std::string to_string() const {
    std::stringstream stream;
    stream << "HTTP/" << versionMajor << "." << versionMinor << " "
           << statusCode << " " << status_str() << "\r\n";

    for (std::vector<Response::HeaderItem>::const_iterator it = headers.begin();
         it != headers.end(); ++it) {
      stream << it->name << ": " << it->value << "\r\n";
    }

    std::string data(content.begin(), content.end());
    stream << "\r\n" << data << "\n";
    return stream.str();
  }

  std::string status_str() const {
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
};

} // namespace httpparser

#endif // HTTPPARSER_RESPONSE_H
