#ifndef __RESTFULPP_PARSER_H__
#define __RESTFULPP_PARSER_H__

#include "RESTfulpp/Request.h"
#include "RESTfulpp/Response.h"
#include "llhttp.h"
#include <cstddef>
#include <map>
#include <string>
#include <vector>

namespace RESTfulpp {
namespace Internals {

struct ParserData {
  ParserData() : method(), url(), key_val_vector(), body(){};
  std::string method;
  std::string url;
  std::vector<std::string> key_val_vector;
  std::vector<char> body;
  unsigned int version_major, version_minor;
  std::function<void(void*)> on_complete;
  void* on_compelete_args;
};

class BaseParser {
public:
  BaseParser();
  ~BaseParser();
  void set_on_complete_cb(std::function<void(void*)> cb, void* args);
  void reset();

protected:
  llhttp_errno _process(const char *data, size_t length);

  ParserData _data;
  llhttp_t parser;
  llhttp_settings_t settings;
};

class RequestParser : BaseParser {
public:
  RequestParser();
  void process(const char *data, size_t length);
  Request parse(const char *data, size_t length);
  // Expose reset and set_on_complete_cb from BaseParser
  using BaseParser::reset;
  using BaseParser::set_on_complete_cb;

  Request snapshot();
};

class ResponseParser : BaseParser {
public:
  ResponseParser();
  void process(const char *data, size_t length);
  Response parse(const char *data, size_t length);

  Response snapshot();

};

} // namespace Internals
} // namespace RESTfulpp
#endif // !__RESTFULPP_PARSER_H__
