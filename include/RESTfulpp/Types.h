#ifndef __RESTFUL_TYPES_H__
#define __RESTFUL_TYPES_H__

#include <string>
#include <unordered_map>
#include <vector>
namespace RESTfulpp {

static std::unordered_map<std::string, std::string>
parseParams(std::string query);

static void
_parse_param_pair(std::string &param_pair, std::string &key, std::string &value,
                  std::unordered_map<std::string, std::string> &value_map);

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

private:
  void parse(std::string url_str);
};
} // namespace RESTfulpp

#endif // !__RESTFUL_TYPES_H__
#define __RESTFUL_TYPES_H__
