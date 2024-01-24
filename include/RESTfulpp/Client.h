#ifndef __RESTFUL_CLIENT_H__
#define __RESTFUL_CLIENT_H__

#include "RESTfulpp/Request.h"
#include "RESTfulpp/Response.h"
#include "RESTfulpp/Types.h"
#include "sockpp/tcp_connector.h"
#include <string>

namespace RESTfulpp {
class Client {
public:
  Client();

  Response send(Request req);

private:
  sockpp::tcp_connector _connector;
};
} // namespace RESTfulpp

#endif // !__RESTFUL_CLIENT_H__
