#ifndef __RESTFULPP_RESPONSE_H__
#define __RESTFULPP_RESPONSE_H__

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>
namespace RESTfulpp {
class Response {
public:
  std::string error;

  unsigned int status_code;
  unsigned int version_major, version_minor;
  std::map<std::string, std::string> headers;
  std::vector<char> content;

  Response();
  Response(unsigned int statusCode, std::string body);

  std::string serialize();
};
} // namespace RESTfulpp

#endif // !__RESTFULPP_RESPONSE_H__
