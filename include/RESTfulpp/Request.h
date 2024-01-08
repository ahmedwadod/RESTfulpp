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
  RESTfulpp::Url url;
  std::unordered_map<std::string, std::string> headers;
  std::unordered_map<std::string, std::string> formData;
  std::vector<char> content;

  Request();
  Request(std::string url);

  std::string body_as_string();
};
} // namespace RESTfulpp

#endif // !__RESTFULPP_REQUEST_H
