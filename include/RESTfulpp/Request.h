#ifndef __RESTFULPP_REQUEST_H
#define __RESTFULPP_REQUEST_H

#include "Types.h"
#include "httpparser/httprequestparser.h"
#include "httpparser/request.h"
#include "httpparser/urlparser.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
namespace RESTfulpp {
class Request {
public:
  std::string error;
  RESTfulpp::Uri uri;
  std::map<std::string, std::string> queryParams;
  std::map<std::string, std::string> formData;
  std::map<std::string, std::string> headers;

  Request(const std::vector<char> &raw_request, size_t length) {
    // Parse the raw request
    auto result = httpparser::HttpRequestParser().parse(
        _raw_req, raw_request.data(), raw_request.data() + length);
    if (result != httpparser::HttpRequestParser::ParsingCompleted) {
      error = "Bad Request";
      return;
    }

    // Parse uri and query params
    httpparser::UrlParser parser("http://localhost" + _raw_req.uri);
    if (!parser.isValid()) {
      error = "Bad URL";
      return;
    }
    uri = Uri(parser.path(), parser.query());
    queryParams = _parse_params(uri.queryStr);

    // Populate headers
    std::for_each(_raw_req.headers.begin(), _raw_req.headers.end(),
                  [&](httpparser::Request::HeaderItem e) {
                    headers.insert(std::make_pair(e.name, e.value));
                  });

    // Body
    if (headers["Content-type"] == "application/x-www-form-urlencoded") {
      formData = _parse_params(body_as_string());
    }

    error = "";
  }

  std::string body_as_string() {
    return std::string(_raw_req.content.begin(), _raw_req.content.end());
  }

private:
  httpparser::Request _raw_req;
  std::map<std::string, std::string> _parse_params(std::string params_str) {
    std::map<std::string, std::string> params;

    if (params_str.empty())
      return params;

    auto i_param = 0;
    auto i_nparam = 0;
    while (i_nparam != std::string::npos) {
      i_nparam = uri.queryStr.find('&');
      if (i_nparam == std::string::npos) {
        params.insert(_param_to_pair(params_str.substr(i_param)));
      } else {
        params.insert(_param_to_pair(params_str.substr(i_param, i_nparam)));
        i_param = i_nparam + 1;
      }
    }

    return params;
  }
  std::pair<std::string, std::string> _param_to_pair(std::string str) {
    std::string key, value;
    auto epos = str.find('=');
    key = str.substr(0, epos);
    value = str.substr(epos + 1);
    return std::make_pair(key, value);
  }
};

} // namespace RESTfulpp

#endif // !__RESTFULPP_REQUEST_H
