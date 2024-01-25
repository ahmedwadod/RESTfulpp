#ifndef __RESTFULPP_SERVER_H__
#define __RESTFULPP_SERVER_H__

#include "RESTfulpp/Types.h"
#include "sockpp/inet_address.h"
#include "sockpp/tcp_acceptor.h"
#include "sockpp/tcp_socket.h"
#include <functional>
#include <map>

namespace RESTfulpp {

typedef std::function<int(sockpp::tcp_socket, sockpp::inet_address)>
    TCPClientHandler;

class Server {
public:
  Server(short port, unsigned int max_request_length = 1024);

  void start(int thread_count = 2);

private:
  unsigned int _max_req_size;
  TCPClientHandler tcpClientHandler;
  sockpp::tcp_acceptor _acceptor;
  std::map<std::string, RouteHandler> _route_handlers;
};
} // namespace RESTfulpp

#endif // !__RESTFULPP_SERVER_H__
