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
  std::function<void()> on_complete;
};

class BaseParser {
public:
  BaseParser(std::function<void()> on_parsing_complete = []() {});
  ~BaseParser();
  void reset();

protected:
  llhttp_errno _process(const char *data, size_t length);
  std::function<void()> _on_parsing_complete;

  ParserData _data;
  llhttp_t parser;
  llhttp_settings_t settings;
};

class RequestParser : BaseParser {
public:
  RequestParser(std::function<void()> on_parsing_complete = []() {});
  void process(const char *data, size_t length);
  Request parse(const char *data, size_t length);

  Request snapshot();
};

class ResponseParser : BaseParser {
public:
  ResponseParser(std::function<void()> on_parsing_complete = []() {});
  void process(const char *data, size_t length);
  Response parse(const char *data, size_t length);

  Response snapshot();

};

} // namespace Internals
} // namespace RESTfulpp
#endif // !__RESTFULPP_PARSER_H__
