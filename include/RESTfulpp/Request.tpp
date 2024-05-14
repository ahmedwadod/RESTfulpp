#include "RESTfulpp/Request.h"
#include <memory>

using namespace RESTfulpp;

template<typename T>
void extraDeleter(void *extra){
  delete (T*)extra;
}

template <typename T> void Request::set_extra(const T &extra) {
  _extra = std::shared_ptr<void>((void *)new T(extra), extraDeleter<T>);
  _extra_size = sizeof(T);
}

template <typename T> T *Request::get_extra() const { return (T *)_extra.get(); }