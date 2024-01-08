#ifndef __RESTFULPP_REQUEST_H
#define __RESTFULPP_REQUEST_H

#include "Types.h"
#include <string>
#include <unordered_map>
#include <vector>
namespace RESTfulpp {
class Request {
public:
  std::string error;

  std::string method;
  RESTfulpp::Url url;
  unsigned int version_major, version_minor;
  std::unordered_map<std::string, std::string> headers;
  std::unordered_map<std::string, std::string> formData;
  std::vector<char> content;

  Request();

  void populate();
  std::string body_as_string();
};
} // namespace RESTfulpp

#endif // !__RESTFULPP_REQUEST_H
