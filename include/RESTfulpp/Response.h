#ifndef __RESTFULPP_RESPONSE_H__
#define __RESTFULPP_RESPONSE_H__

#include "nlohmann/json.hpp"
#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>
namespace RESTfulpp {
class Response {
public:
  int status_code;
  int version_major, version_minor;
  std::map<std::string, std::string> headers;
  std::vector<char> content;

  Response();

  static Response plaintext(unsigned int status_code, std::string text);
  static Response html(unsigned int status_code, std::string html);
  static Response json(unsigned int status_code, nlohmann::json json_data);

  std::string body() const;
  nlohmann::json body_as_json() const;
  std::string serialize() const;
  bool is_response_keep_alive() const;
  void set_response_keep_alive(bool keep_alive, int timeout);

};
} // namespace RESTfulpp

#endif // !__RESTFULPP_RESPONSE_H__
