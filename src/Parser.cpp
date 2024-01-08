#include "RESTfulpp/Parser.h"
#include "RESTfulpp/Request.h"
#include "llhttp.h"
#include <cstddef>
#include <mutex>
#include <string>
#include <vector>

using namespace RESTfulpp;

BaseParser::BaseParser() {
  llhttp_settings_init(&settings);

  settings.on_method = [](llhttp_t *p, const char *at, unsigned long length) {
    __method = std::string(at, length);
    return 0;
  };

  settings.on_url = [](llhttp_t *p, const char *at, unsigned long length) {
    __url = std::string(at, length);
    return 0;
  };

  settings.on_header_field = [](llhttp_t *p, const char *at,
                                unsigned long length) {
    __key_val_vector.emplace_back(at, length);
    return 0;
  };

  settings.on_header_value = [](llhttp_t *p, const char *at,
                                unsigned long length) {
    __key_val_vector.emplace_back(at, length);
    return 0;
  };

  settings.on_body = [](llhttp_t *p, const char *at, unsigned long length) {
    __body = std::string(at, length);
    return 0;
  };

  llhttp_init(&parser, HTTP_BOTH, &settings);
}

llhttp_errno BaseParser::_parse(const char *data, unsigned long length) {
  __method.clear();
  __url.clear();
  __key_val_vector.clear();
  __body.clear();
  llhttp_reset(&parser);

  auto err = llhttp_execute(&parser, data, length);
  if (err == HPE_OK) {
    for (auto i = 0; i < __key_val_vector.size(); i += 2) {
      headers[__key_val_vector[i]] = __key_val_vector[i + 1];
    }
    content = std::vector<char>(__body.begin(), __body.end());
  }

  return err;
}

RequestParser::RequestParser() : BaseParser() {}

Request RequestParser::parse(const char *data, size_t length) {
  std::lock_guard<std::mutex> guard(__parse_mutex);
  Request req;
  if (_parse(data, length) != HPE_OK) {
    req.error = llhttp_get_error_reason(&parser);
    return req;
  }

  req.method = __method;
  req.url = Url(__url);
  req.version_major = llhttp_get_http_major(&parser);
  req.version_minor = llhttp_get_http_minor(&parser);
  req.headers = headers;
  req.content = content;
  req.populate();

  return req;
}

Request RequestParser::parse(std::vector<char> data) {
  return parse(data.data(), data.size());
}
