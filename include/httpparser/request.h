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

  std::map<std::string, std::string> get_query_params() {
    std::map<std::string, std::string> params;
    UrlParser url("http://localhost" + uri);
    if (!url.isValid() || url.query().length() == 0)
      return params;

    auto query_str = url.query();

    auto itr_param = 0;
    auto i_nparam = 0;
    while (i_nparam != std::string::npos) {
      i_nparam = query_str.find('&');
      if (i_nparam == std::string::npos) {
        params.insert(_param_to_pair(query_str.substr(itr_param)));
      } else {
        params.insert(_param_to_pair(query_str.substr(itr_param, i_nparam)));
        itr_param = i_nparam + 1;
      }
    }

    return params;
  }

private:
  std::pair<std::string, std::string> _param_to_pair(std::string str) {
    std::string key, value;
    auto epos = str.find('=');
    key = str.substr(0, epos);
    value = str.substr(epos + 1);
    return std::make_pair(key, value);
  }
};

} // namespace httpparser

#endif // HTTPPARSER_REQUEST_H
