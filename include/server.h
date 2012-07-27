#include <string>
#include <netinet/in.h>

#include "http/middleware.h"
#include "util/exception.h"

namespace lhs {
  class server_error {};
  class server {
    public:
    struct peer {
      int port;
      char ip[INET6_ADDRSTRLEN];
    };

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
      struct peer getpeer(int socket);

    public:
      static void *client_thread_helper(void *context); 

    private:
      std::string addr_;
      int port_;
      int socket_;
      struct sockaddr_in sockaddr_;
  };
}
