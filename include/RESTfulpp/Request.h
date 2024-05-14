#ifndef __RESTFULPP_REQUEST_H
#define __RESTFULPP_REQUEST_H

#include "Util.h"
#include "nlohmann/json.hpp"
#include <map>
#include <string>
#include <vector>
namespace RESTfulpp {
class Request {
public:
  std::string method;
  RESTfulpp::Uri uri;
  unsigned int version_major, version_minor;
  std::map<std::string, std::string> headers;
  std::map<std::string, std::string> url_params;
  std::string client_ip;
  std::vector<char> content;

  Request();
  ~Request();

  static Request from_form_data(std::map<std::string, std::string> form_data);

  std::map<std::string, std::string> body_as_form_data() const;
  std::string body_as_text() const;
  nlohmann::json body_as_json() const;
  std::map<std::string, std::string> cookies() const;

  std::string serialize() const;
  bool is_request_keep_alive() const;
  void set_request_keep_alive(bool keep_alive,
                              int timeout = DEFAULT_CONN_TIMEOUT_SEC);

  // Extra payload

  /*
   * @brief This function is used to set extra payload to the request object it
   * will override the previous extra payload if any and will free the memory of
   * the previous extra payload
   * @param extra: The extra payload to be set
   * @return void
   */
  template <typename T> void set_extra(T extra);

  /*
   * @brief This function is used to get the extra payload from the request
   * object
   * @return T*: Pointer to the extra payload
   */
  template <typename T> T *get_extra() const;

  /*
   * @brief This function is used to get the size of the extra payload
   * @return size_t: The size of the extra payload
   */
  size_t get_extra_size() const;

private:
  size_t _extra_size = 0;
  void *_extra = NULL;
};
} // namespace RESTfulpp

#include "Request.tpp"

#endif // !__RESTFULPP_REQUEST_H
