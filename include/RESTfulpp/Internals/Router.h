#ifndef __REFTFULPP_ROUTER_H__
#define __REFTFULPP_ROUTER_H__

#include "RESTfulpp/Util.h"
#include "RESTfulpp/Request.h"
#include <map>
#include <optional>
#include <regex>
#include <string>
#include <vector>

namespace RESTfulpp {
namespace Internals {

namespace Router {

RESTfulpp::RouteDefinition route_str_to_definition(std::string str);

std::optional<std::map<std::string, std::string>>
match_route(RESTfulpp::RouteDefinition def, std::string str);

std::optional<std::map<std::string, std::string>>
match_request(RESTfulpp::RouteDefinition def, const RESTfulpp::Request &req);

} // namespace Router
} // namespace Internals
} // namespace RESTfulpp
#endif // !__REFTFULPP_ROUTER_H__
