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
  std::vector<char> content;

  Request();

  static Request from_form_data(std::map<std::string, std::string> form_data);

  std::map<std::string, std::string> get_form_data() const;
  std::string body() const;
  nlohmann::json json() const;

  std::string serialize() const;
};
} // namespace RESTfulpp

#endif // !__RESTFULPP_REQUEST_H
