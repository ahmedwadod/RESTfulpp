#ifndef __REFTFULPP_ROUTER_H__
#define __REFTFULPP_ROUTER_H__

#include "../Types.h"
#include <map>
#include <optional>
#include <regex>
#include <string>
#include <vector>

namespace RESTfulpp {
namespace Internals {

namespace Router {

struct RouteDefinition {
  std::string route_name;
  std::string method;
  std::vector<std::string> params_names;
  std::regex route_regex;
  RESTfulpp::RouteHandler handler;
};

RouteDefinition route_str_to_definition(std::string str);

std::optional<std::map<std::string, std::string>>
match_route(RouteDefinition def, std::string str);

std::optional<std::map<std::string, std::string>>
match_request(RouteDefinition def, RESTfulpp::Request req);

} // namespace Router
} // namespace Internals
} // namespace RESTfulpp
#endif // !__REFTFULPP_ROUTER_H__
