#ifndef __RESTFULPP_REQUEST_H
#define __RESTFULPP_REQUEST_H

#include "Types.h"
#include <map>
#include <string>
#include <vector>
namespace RESTfulpp {
class Request {
public:
  std::string method;
  RESTfulpp::Uri uri;
  unsigned int version_major, version_minor;
  std::map<std::string, std::string> headers;
  std::vector<char> content;

  Request();

  std::string serialize();
};
} // namespace RESTfulpp

#endif // !__RESTFULPP_REQUEST_H
