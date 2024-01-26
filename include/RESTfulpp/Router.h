#ifndef __REFTFULPP_ROUTER_H__
#define __REFTFULPP_ROUTER_H__

#include <map>
#include <optional>
#include <regex>
#include <string>
#include <vector>

namespace RESTfulpp {
namespace Router {

struct RouteDefinition {
  std::string route_name;
  std::vector<std::string> params_names;
  std::regex route_regex;
};

RouteDefinition route_str_to_regex(std::string str);

std::optional<std::map<std::string, std::string>>
match_route(RouteDefinition def, std::string str);

} // namespace Router
} // namespace RESTfulpp
#endif // !__REFTFULPP_ROUTER_H__
