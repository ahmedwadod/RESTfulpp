#ifndef __RESTFULPP_PARSER_H__
#define __RESTFULPP_PARSER_H__

#include "RESTfulpp/Request.h"
#include "llhttp.h"
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace RESTfulpp {
struct ParserData {
  ParserData() : method(), url(), key_val_vector(), body(){};
  std::string method;
  std::string url;
  std::vector<std::string> key_val_vector;
  std::string body;
};

class BaseParser {
public:
  BaseParser();

protected:
  std::unordered_map<std::string, std::string> headers;
  std::vector<char> content;
  llhttp_errno _parse(const char *data, unsigned long length);

  ParserData _data;
  llhttp_t parser;
  llhttp_settings_t settings;
};

class RequestParser : BaseParser {
public:
  RequestParser();
  Request parse(const char *data, size_t length);
  Request parse(std::vector<char> raw_data);
};
} // namespace RESTfulpp

#endif // !__RESTFULPP_PARSER_H__
