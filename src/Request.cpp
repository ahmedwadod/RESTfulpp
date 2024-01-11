#include "RESTfulpp/Request.h"
#include "RESTfulpp/Types.h"
#include <algorithm>
#include <ios>
#include <sstream>
#include <string>
#include <vector>

using namespace RESTfulpp;

Request::Request() {}
Request::Request(std::string method, std::string path, std::vector<char> body)
    : method(method), version_major(1), version_minor(1), url(path),
      content(body) {}

void Request::populate() {
  if (headers["Content-Type"] == "application/x-www-form-urlencoded") {
    formData = parseParams(std::string(content.begin(), content.end()));
  }
}

std::string Request::serialize() {
  std::stringstream s;
  std::string path = "/";
  if (!url.path.empty())
    path = url.path;
  s << method << " " << path;
  if (!url.query.empty()) {
    s << "?";
    std::for_each(
        url.query_params.rbegin(), url.query_params.rend(),
        [&s](auto param) { s << param.first << "=" << param.second << "&"; });
    s.seekp(-1, std::ios_base::end);
  }

  s << " HTTP/" << version_major << "." << version_minor << "\r\n";

  std::for_each(headers.rbegin(), headers.rend(), [&s](auto param) {
    s << param.first << ": " << param.second << "\r\n";
  });

  s << "\r\n" << std::string(content.begin(), content.end());

  return s.str();
}
