#include "RESTfulpp/Types.h"
#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <regex>
#include <string>
#include <unordered_map>

using namespace RESTfulpp;

Url::Url() {}
Url::Url(std::string url_str) {
  parse(url_str);
  query_params = parseParams(query);
}

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
  }
}

static std::unordered_map<std::string, std::string>
RESTfulpp::parseParams(std::string query) {
  std::unordered_map<std::string, std::string> params;
  std::string param_pair;
  std::string key, value;
  for (char c : query) {
    if (c == '&') {
      _parse_param_pair(param_pair, key, value, params);
      param_pair.clear();
    } else {
      param_pair += c;
    }
  }
  // Parse the last parameter pair
  _parse_param_pair(param_pair, key, value, params);

  return params;
}
// Helper function to parse a key-value pair from the query string
static void RESTfulpp::_parse_param_pair(
    std::string &param_pair, std::string &key, std::string &value,
    std::unordered_map<std::string, std::string> &value_map) {
  size_t eq_pos = param_pair.find('=');
  if (eq_pos != std::string::npos) {
    key = param_pair.substr(0, eq_pos);
    value = param_pair.substr(eq_pos + 1);
    value_map[key] = value;
  } else {
    // Handle parameter without a value
    key = param_pair;
    value_map[key] = "";
  }
}
