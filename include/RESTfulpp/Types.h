#ifndef __RESTFUL_TYPES_H__
#define __RESTFUL_TYPES_H__

#include <string>
#include <unordered_map>
#include <vector>
namespace RESTfulpp {

// Helper function to parse a key-value pair from the query string
static void
_parse_param_pair(std::string &param_pair, std::string &key, std::string &value,
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

static std::unordered_map<std::string, std::string>
parseParams(std::string query) {
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

class Url {
public:
  std::string protocol;
  std::string host;
  std::string port;
  std::string path;
  std::string query;
  std::unordered_map<std::string, std::string> query_params;

  Url();
  Url(std::string url_str);
  bool is_valid();

private:
  bool error = true;
  void parse(std::string url_str);
};
} // namespace RESTfulpp

#endif // !__RESTFUL_TYPES_H__
#define __RESTFUL_TYPES_H__
