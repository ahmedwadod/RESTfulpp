#include "RESTfulpp/Request.h"
#include <string>

using namespace RESTfulpp;

Request::Request() {}

void Request::populate() {
  if (headers["Content-Type"] == "application/x-www-form-urlencoded") {
    formData = parseParams(std::string(content.begin(), content.end()));
  }
}
