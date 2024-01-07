#include "../include/RESTfulpp/Server.h"
#include "sockpp/tcp_socket.h"
#include <iostream>

int main() {
  RESTfulpp::Server s(8080);

  s.start();

  return 0;
}
