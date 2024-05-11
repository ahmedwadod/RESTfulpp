#include "RESTfulpp/Internals/Parser.h"
#include "RESTfulpp/Request.h"
#include "RESTfulpp/Response.h"
#include "llhttp.h"
#include <cstddef>
#include <string>
#include <vector>

using namespace RESTfulpp;
using namespace RESTfulpp::Internals;

BaseParser::BaseParser(std::function<void()> on_parsing_complete)
    : _on_parsing_complete(on_parsing_complete) {
  llhttp_settings_init(&settings);

  _data = ParserData();
  _data.on_complete = [&]() { _on_parsing_complete(); };

  settings.on_message_begin = [](llhttp_t *p) {
    auto parserData = static_cast<ParserData *>(p->data);
    parserData->method.clear();
    parserData->url.clear();
    parserData->key_val_vector.clear();
    parserData->body.clear();
    return 0;
  };

  settings.on_method = [](llhttp_t *p, const char *at, size_t length) {
    ((ParserData *)p->data)->method = std::string(at, length);
    return 0;
  };

  settings.on_version_complete = [](llhttp_t *p) {
    auto parserData = static_cast<ParserData *>(p->data);
    parserData->version_major = llhttp_get_http_major(p);
    parserData->version_minor = llhttp_get_http_minor(p);
    return 0;
  };

  settings.on_url = [](llhttp_t *p, const char *at, size_t length) {
    ((ParserData *)p->data)->url = std::string(at, length);
    return 0;
  };

  settings.on_header_field = [](llhttp_t *p, const char *at, size_t length) {
    ((ParserData *)p->data)->key_val_vector.emplace_back(at, length);
    return 0;
  };

  settings.on_header_value = [](llhttp_t *p, const char *at, size_t length) {
    ((ParserData *)p->data)->key_val_vector.emplace_back(at, length);
    return 0;
  };

  settings.on_body = [](llhttp_t *p, const char *at, size_t length) {
    ((ParserData *)p->data)->body.insert(((ParserData *)p->data)->body.end(), at, at + length);
    return 0;
  };

  settings.on_message_complete = [](llhttp_t *p) {
    auto parserData = static_cast<ParserData *>(p->data);
    parserData->on_complete();
    return 0;
  };

  llhttp_init(&parser, HTTP_BOTH, &settings);
  parser.data = (void *)&_data;
}

void BaseParser::reset() {
  llhttp_init(&parser, HTTP_BOTH, &settings);
  _data.method.clear();
  _data.url.clear();
  _data.key_val_vector.clear();
  _data.body.clear();
  parser.data = (void *)&_data;
}

BaseParser::~BaseParser() {}

llhttp_errno BaseParser::_process(const char *data, size_t length) {
  return llhttp_execute(&parser, data, length);
}

// ======== RequestParser ========
RequestParser::RequestParser(std::function<void()> on_parsing_complete)
    : BaseParser(on_parsing_complete) {}

Request RequestParser::snapshot() {

  Request req;
  req.method = _data.method;
  req.uri = Uri(_data.url);
  req.version_major = _data.version_major;
  req.version_minor = _data.version_minor;
  for (auto i = 0; i < _data.key_val_vector.size(); i += 2)
    req.headers[_data.key_val_vector[i]] = _data.key_val_vector[i + 1];
  req.content = std::vector<char>(_data.body.begin(), _data.body.end());

  return req;
}

void RequestParser::process(const char *data, size_t length) {
  if (_process(data, length) != HPE_OK)
    throw llhttp_get_error_reason(&parser);
}

Request RequestParser::parse(const char *data, size_t length) {
  process(data, length);
  return snapshot();
}

// ======== End of RequestParser ========

// ======== ResponseParser ========
ResponseParser::ResponseParser(std::function<void()> on_parsing_complete)
    : BaseParser(on_parsing_complete) {}

Response ResponseParser::snapshot() {
  Response res;
  res.status_code = llhttp_get_status_code(&parser);
  res.version_major = _data.version_major;
  res.version_minor = _data.version_minor;
  for (auto i = 0; i < _data.key_val_vector.size(); i += 2)
    res.headers[_data.key_val_vector[i]] = _data.key_val_vector[i + 1];
  res.content = std::vector<char>(_data.body.begin(), _data.body.end());

  return res;
}

void ResponseParser::process(const char *data, size_t length) {
  if (_process(data, length) != HPE_OK)
    throw llhttp_get_error_reason(&parser);
}

Response ResponseParser::parse(const char *data, size_t length) {
  if (_process(data, length) != HPE_OK)
    throw llhttp_get_error_reason(&parser);

  return snapshot();
}
