#ifndef __RESTFULPP_REQUEST_H
#define __RESTFULPP_REQUEST_H

#include "Types.h"
#include "nlohmann/json.hpp"
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
  std::map<std::string, std::string> url_params;
  std::string client_ip;
  std::vector<char> content;

  Request();

  static Request from_form_data(std::map<std::string, std::string> form_data);

  std::map<std::string, std::string> body_as_form_data() const;
  std::string body_as_text() const;
  nlohmann::json body_as_json() const;

  std::string serialize() const;
};
} // namespace RESTfulpp

#endif // !__RESTFULPP_REQUEST_H
