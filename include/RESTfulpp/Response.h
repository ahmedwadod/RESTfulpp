#ifndef __RESTFULPP_RESPONSE_H__
#define __RESTFULPP_RESPONSE_H__

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
namespace RESTfulpp {
class Response {
public:
  unsigned int status_code;
  std::unordered_map<std::string, std::string> headers;

  Response(std::string raw_resp);
};
} // namespace RESTfulpp

#endif // !__RESTFULPP_RESPONSE_H__
