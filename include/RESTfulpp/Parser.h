#ifndef __RESTFULPP_PARSER_H__
#define __RESTFULPP_PARSER_H__

#include "RESTfulpp/Request.h"
#include "llhttp.h"
#include <cstddef>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace RESTfulpp {

static std::mutex __parse_mutex;
static std::string __method;
static std::string __url;
static std::vector<std::string> __key_val_vector;
static std::string __body;

class BaseParser {
public:
  BaseParser();

protected:
  std::unordered_map<std::string, std::string> headers;
  std::vector<char> content;
  llhttp_errno _parse(const char *data, unsigned long length);

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
