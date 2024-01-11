#include "RESTfulpp/Types.h"
#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <regex>
#include <string>

using namespace RESTfulpp;

Url::Url() {}
Url::Url(std::string url_str) {
  parse(url_str);
  if (error)
    parse("http://localhost:8080" + url_str);
  if (!error)
    query_params = parseParams(query);
}

bool Url::is_valid() { return !error; }

void Url::parse(std::string url_str) {
  // Regular expression to match URL components
  std::regex url_regex(
      R"(^(\w+):\/\/([^:/]+)(:(\d+))?([^?#]*)(\?([^#]*))?(#(.*))?)");
  std::smatch matches;

  // Match the URL against the regular expression
  if (std::regex_match(url_str, matches, url_regex)) {
    protocol = matches[1];
    host = matches[2];
    path = matches[5].str();
    query = matches[7].str();

    // Extract port if present
    if (!matches[4].str().empty()) {
      port = matches[4];
    }
    error = false;
  } else {
    error = true;
  }
}
