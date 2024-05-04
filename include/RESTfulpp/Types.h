#ifndef __RESTFUL_TYPES_H__
#define __RESTFUL_TYPES_H__

/*
 * RESTful++ - Types.h
 *
 * This is the definitions of the common types and functions.
 *
 * Note that stuff like headers, form data, etc.. are not defined
 * as type but rather the standard C++ types are used.
 *
 */

#include <functional>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace RESTfulpp
{

  // These are defined here to avoid circular dependancy with Request and Response
  class Response;
  class Request;

  // These 2 functions are used with both form data and query params
  void _parse_param_pair(std::string &param_pair, std::string &key,
                         std::string &value,
                         std::map<std::string, std::string> &value_map);

  std::map<std::string, std::string> parseParams(std::string query, char seprator = '&');

  // Uri (The path after the host and port)
  class Uri
  {
  public:
    std::string path;
    std::map<std::string, std::string> query_params;

    Uri();
    Uri(std::string path, std::map<std::string, std::string> params);
    Uri(std::string uri_str);

    std::string to_string() const;
  };

  // Full Url containing the Uri as RESTfulpp::Uri type
  class Url
  {
  public:
    std::string protocol;
    std::string host;
    std::string port;
    Uri uri;

    Url(std::string url_str);

  private:
    std::string path;
    std::string query;
    void parse(std::string url_str);
  };

// Methods (Unused for now)
#define ANY "ANY"
#define GET "GET"
#define HEAD "HEAD"
#define POST "POST"
#define PUT "PUT"
#define DELETE "DELETE"
#define CONNECT "CONNECT"
#define OPTIONS "OPTIONS"
#define TRACE "TRACE"
#define PATCH "PATCH"

  // Functions that are used with the route definitions
  typedef std::function<Response(Request)> RouteHandler;
  typedef std::function<Response(Request, RouteHandler)> MiddlewareHandler;
  typedef std::variant<RouteHandler, MiddlewareHandler> RouteFunction;

} // namespace RESTfulpp

#endif // !__RESTFUL_TYPES_H__
#define __RESTFUL_TYPES_H__
