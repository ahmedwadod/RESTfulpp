/*
 * Copyright (C) Alex Nekipelov (alex@nekipelov.net)
 * License: MIT
 *
 * Code modified by Ahmed Elkhalifa (ahmed@elkhalifa.dev)
 */

#ifndef HTTPPARSER_REQUEST_H
#define HTTPPARSER_REQUEST_H

#include "httpparser/urlparser.h"
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace httpparser {

struct Request {
  Request() : versionMajor(0), versionMinor(0), keepAlive(false) {}

  struct HeaderItem {
    std::string name;
    std::string value;
  };

  std::string method;
  std::string uri;
  int versionMajor;
  int versionMinor;
  std::vector<HeaderItem> headers;
  std::vector<char> content;
  bool keepAlive;

  std::string to_string() const {
    std::stringstream stream;
    stream << method << " " << uri << " HTTP/" << versionMajor << "."
           << versionMinor << "\r\n";

    for (std::vector<Request::HeaderItem>::const_iterator it = headers.begin();
         it != headers.end(); ++it) {
      stream << it->name << ": " << it->value << "\r\n";
    }

    std::string data(content.begin(), content.end());
    stream << "\r\n" << data << "\n";
    stream << "+ keep-alive: " << keepAlive << "\r\n";
    ;
    return stream.str();
  }
};

} // namespace httpparser

#endif // HTTPPARSER_REQUEST_H
