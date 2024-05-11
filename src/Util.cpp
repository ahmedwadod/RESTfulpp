#include "RESTfulpp/Util.h"
#include "RESTfulpp/Request.h"
#include "RESTfulpp/Response.h"
#include "RESTfulpp/Internals/Router.h"
#include <ios>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace RESTfulpp;

// URI
Uri::Uri(){};
Uri::Uri(std::string uri_str) {
  if (uri_str.empty()) {
    path = "/";
    return;
  }

  if (uri_str[0] != '/' && uri_str[0] != '?')
    throw "Invalid Uri at position 0";

  auto paramsPos = uri_str.find_first_of('?');
  path = uri_str.substr(0, paramsPos);
  if (path.empty())
    path = '/';
  if (paramsPos != std::string::npos)
    query_params = parseParams(uri_str.substr(paramsPos + 1));
}

Uri::Uri(std::string path_str, std::map<std::string, std::string> query)
    : query_params(query) {
  path = path_str.empty() ? "/" : path_str;
};

std::string Uri::to_string() const {
  std::stringstream s;
  s << path;
  if (!query_params.empty()) {
    s << '?';
    for (auto q : query_params)
      s << q.first << '=' << q.second << '&';
    s.seekp(-1, std::ios_base::end);
  }
  return s.str();
}

// URL
Url::Url(std::string url_str) {
  parse(url_str);
  uri = Uri(path, parseParams(query));
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
    if (!matches[4].str().empty())
      port = matches[4];
    else
      port = protocol == "https" ? "443" : "80";
  } else {
    throw "Invalid URL";
  }
}

// Helper function to parse a key-value pair from the query string
void RESTfulpp::_parse_param_pair(
    std::string &param_pair, std::string &key, std::string &value,
    std::map<std::string, std::string> &value_map) {
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

std::map<std::string, std::string> RESTfulpp::parseParams(std::string query,
                                                          char seprator) {
  std::map<std::string, std::string> params;
  std::string param_pair;
  std::string key, value;
  for (char c : query) {
    if (c == seprator) {
      _parse_param_pair(param_pair, key, value, params);
      param_pair.clear();
    } else {
      // if param_pair is empty and c is a space, ignore it
      if (param_pair.empty() && c == ' ')
        continue;
      param_pair += c;
    }
  }

  // Parse the last parameter pair if it exists
  if (!param_pair.empty())
    _parse_param_pair(param_pair, key, value, params);

  return params;
}

Response RESTfulpp::process_request_with_routes(Request req,
                                     std::vector<RouteDefinition> *routes) {
  for (auto route : *routes) {
    auto match = Internals::Router::match_request(route, req);
    if (match.has_value()) {
      try {
        req.url_params = match.value();
        return route.handler(req);
      } catch (std::exception &e) {
        return Response::plaintext(500, "Internal Server Error");
      }
    }
  }

  return Response::plaintext(404, "Not Found");
}
