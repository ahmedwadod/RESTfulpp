#ifndef __RESTFULPP_REQUEST_H
#define __RESTFULPP_REQUEST_H

#include "Types.h"
#include <map>
#include <string>
#include <vector>
namespace RESTfulpp {
class Request {
public:
  std::string error;

  std::string method;
  RESTfulpp::Url url;
  unsigned int version_major, version_minor;
  std::map<std::string, std::string> headers;
  std::map<std::string, std::string> formData;
  std::vector<char> content;

  Request();
  Request(std::string method, std::string url, std::vector<char> body);

  void populate();
  std::string serialize();
};
} // namespace RESTfulpp

#endif // !__RESTFULPP_REQUEST_H
