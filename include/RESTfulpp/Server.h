#ifndef __RESTFULPP_SERVER_H__
#define __RESTFULPP_SERVER_H__

#include "sockpp/tcp_acceptor.h"
#include "sockpp/tcp_socket.h"
#include <functional>

namespace RESTfulpp {

typedef std::function<int(sockpp::tcp_socket)> TCPClientHandler;

class Server {
public:
  TCPClientHandler tcpClientHandler;

  Server(short port, unsigned int max_request_length = 1024);

  void start();

private:
  unsigned int _max_req_size;
  sockpp::tcp_acceptor _acceptor;
};
} // namespace RESTfulpp

#endif // !__RESTFULPP_SERVER_H__
