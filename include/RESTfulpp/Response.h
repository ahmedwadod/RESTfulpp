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
  unsigned int status_code;
  unsigned int version_major, version_minor;
  std::map<std::string, std::string> headers;
  std::vector<char> content;

  Response();

  static Response plaintext(unsigned int status_code, std::string text);
  static Response html(unsigned int status_code, std::string html);

  std::string serialize() const;
};
} // namespace RESTfulpp

#endif // !__RESTFULPP_RESPONSE_H__
