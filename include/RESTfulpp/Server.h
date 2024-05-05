#ifndef __RESTFULPP_SERVER_H__
#define __RESTFULPP_SERVER_H__

#include "RESTfulpp/Internals/ThreadPool.h"
#include "RESTfulpp/Internals/Router.h"
#include "Types.h"
#include "sockpp/inet_address.h"
#include "sockpp/tcp_acceptor.h"
#include "sockpp/tcp_socket.h"
#include <condition_variable>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>
#include "RESTfulpp/Types.h"

namespace RESTfulpp
{

  // Server
  class Server
  {
  public:
    Server(short port, unsigned int max_request_length = 1024);
    ~Server();

    void any(std::string route_template, RouteHandler func);
    void get(std::string route_template, RouteHandler func);
    void post(std::string route_template, RouteHandler func);
    void put(std::string route_template, RouteHandler func);
    void Delete(std::string route_template, RouteHandler func);
    void patch(std::string route_template, RouteHandler func);
    void head(std::string route_template, RouteHandler func);
    void options(std::string route_template, RouteHandler func);
    void trace(std::string route_template, RouteHandler func);
    void connect(std::string route_template, RouteHandler func);
    void start(int thread_count = std::thread::hardware_concurrency());

  private:
    ThreadPool *_pool;
    unsigned int _max_req_size;
    TCPClientHandler tcpClientHandler;
    sockpp::tcp_acceptor _acceptor;
    void _route(std::string method, std::string route_template,
                RouteHandler func);
    std::vector<Internals::Router::RouteDefinition> _route_definitions;
  };
} // namespace RESTfulpp

#endif // !__RESTFULPP_SERVER_H__
