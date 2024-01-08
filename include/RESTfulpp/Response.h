#ifndef __RESTFULPP_RESPONSE_H__
#define __RESTFULPP_RESPONSE_H__

#include "httpparser/response.h"
#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>
namespace RESTfulpp {
class Response {
public:
  std::map<std::string, std::string> headers;

  Response(std::string raw_resp) {}

  Response() : _raw_res(){};

  Response(unsigned int statusCode) : _raw_res(statusCode) {}

  Response(unsigned int statusCode, std::string body) : _raw_res(statusCode) {
    headers["Content-type"] = "text/plain";
    _raw_res.content = std::vector<char>(body.begin(), body.end());
  }

  Response(unsigned int statusCode, std::string contentType,
           std::vector<char> body)
      : _raw_res(statusCode) {
    headers["Content-type"] = contentType;
    _raw_res.content = body;
  }

  std::string serialize() {
    // Update headers
    _raw_res.headers.clear();
    std::for_each(headers.begin(), headers.end(),
                  [&](std::pair<std::string, std::string> e) {
                    _raw_res.headers.push_back({e.first, e.second});
                  });
    return _raw_res.to_string();
  }

private:
  httpparser::Response _raw_res;
};
} // namespace RESTfulpp

#endif // !__RESTFULPP_RESPONSE_H__
