#include "protocol/http/middleware.h"
#include "util/exception.h"

#include <string>
#if defined _WIN32 || defined __WIN32__ || defined WIN32
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <netinet/in.h>
#endif

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 46
#endif

namespace lhs {
  class server_error {};
  struct peer {
    int port;
    char ip[INET6_ADDRSTRLEN];
  };
  class server {
    public:
      server(const std::string & addr, int port);
      server(int port);
      server();

      void init();
      void init(int max_conn);

      void run();
      void run(lhs::http::middleware *app);
      void quit();

      int port();
      const std::string address();

    private:
      void * client_thread(void *data);
      struct peer getpeer(struct sockaddr_in &client);

    public:
      static void *client_thread_helper(void *context); 

    private:
      std::string addr_;
      int port_;
#if defined _WIN32 || defined __WIN32__ || defined WIN32
      WSADATA wsa_;
      SOCKET socket_;
#else
      int socket_;
#endif
      struct sockaddr_in sockaddr_;
  };
}
