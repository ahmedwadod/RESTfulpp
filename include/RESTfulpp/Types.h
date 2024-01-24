#ifndef __RESTFUL_TYPES_H__
#define __RESTFUL_TYPES_H__

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace RESTfulpp {

class Response;
class Request;

static void _parse_param_pair(std::string &param_pair, std::string &key,
                              std::string &value,
                              std::map<std::string, std::string> &value_map);

static std::map<std::string, std::string> parseParams(std::string query);

class Uri {
public:
  std::string path;
  std::map<std::string, std::string> query_params;

  Uri();
  Uri(std::string path, std::map<std::string, std::string> params);
  Uri(std::string uri_str);

  std::string to_string();
};

class Url {
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

// Methods
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

typedef std::function<Response(Request, std::string, std::string)> RouteHandler;
typedef std::function<Response(Request, std::string, std::string, RouteHandler)>
    MiddlewareHandler;

} // namespace RESTfulpp

#endif // !__RESTFUL_TYPES_H__
#define __RESTFUL_TYPES_H__
