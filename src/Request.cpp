#include "RESTfulpp/Request.h"
#include "RESTfulpp/Types.h"
#include <algorithm>
#include <ios>
#include <sstream>
#include <string>
#include <vector>

using namespace RESTfulpp;

Request::Request() : version_major(1), version_minor(1) {}

std::string Request::serialize() {
  std::stringstream s;
  s << method << " " << uri.to_string() << " ";
  s << "HTTP/" << version_major << "." << version_minor << "\r\n";

  std::for_each(headers.rbegin(), headers.rend(), [&s](auto param) {
    s << param.first << ": " << param.second << "\r\n";
  });

  s << "\r\n" << std::string(content.begin(), content.end());

  return s.str();
}
