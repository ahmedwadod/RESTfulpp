#include "RESTfulpp/Request.h"
using namespace RESTfulpp;

template <typename T> void Request::set_extra(T extra) {
  if (_extra != NULL)
    free(_extra);

  _extra = malloc(sizeof(T));
  _extra_size = sizeof(T);
  memcpy(_extra, &extra, sizeof(T));
}

template <typename T> T *Request::get_extra() const { return (T *)_extra; }