#ifndef __RESTFUL_CLIENT_H__
#define __RESTFUL_CLIENT_H__

#include "RESTfulpp/Request.h"
#include "RESTfulpp/Response.h"
#include "RESTfulpp/Types.h"
#include <string>

namespace RESTfulpp {
class Client {
public:
  Client();

  Response send(Request req);
};
} // namespace RESTfulpp

#endif // !__RESTFUL_CLIENT_H__
