#include "server.h"
#include "http/env.h"
#include "http/request.h"
#include "http/response.h"
#include "util/string.h"
#include "util/datetime.h"

#include <vector>
#include <iostream>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <cstdio>
#include <errno.h>

#define DEFAULT_PORT 2009
#define MAX_CONNECTIONS 1
#define BUFFER_SIZE 1024

struct t_server_data {
  int connection;
  struct lhs::server::peer client;
  lhs::server::server *server;
  lhs::http::middleware *app;
};

lhs::server::server(const std::string & addr, int port) : addr_(addr), port_(port) {
  socket_ = -1;
}
lhs::server::server(int port) : port_(port) {
  socket_ = -1;
  addr_ = "";
}
lhs::server::server() {
  socket_ = -1;
  addr_ = "";
  port_ = DEFAULT_PORT;
}

void lhs::server::init() {
  init(MAX_CONNECTIONS);
}
void lhs::server::init(int max_conn) {
  sockaddr_.sin_family = AF_INET;
  sockaddr_.sin_port = ntohs(port_);
  if(addr_.empty()) {
    sockaddr_.sin_addr.s_addr = htonl(INADDR_ANY);
  } else {
    const char *addr_c = addr_.c_str();
    inet_aton(addr_c, (in_addr*)&sockaddr_.sin_addr.s_addr);
  }
  memset(sockaddr_.sin_zero, 0, sizeof(sockaddr_.sin_zero));

  if(0 > (socket_ = ::socket(AF_INET, SOCK_STREAM, 0))) {
    THROW(server_error, strerror(errno));
  }

  if(0 > ::bind(socket_, (struct sockaddr*)&sockaddr_, sizeof(struct sockaddr))) {
    THROW(server_error, strerror(errno));
  }

  if(0 > ::listen(socket_, max_conn)) {
    THROW(server_error, strerror(errno));
  }
}

void lhs::server::run() {
  run(NULL);
}
void lhs::server::run(lhs::http::middleware *app) {
  while(true) {
    socklen_t max_size;

    int sockfd = ::accept(socket_, (struct sockaddr*)&sockaddr_, &max_size);
    if(0 > sockfd) {
      THROW(server_error, strerror(errno));
    }

    pthread_t thread;
    struct t_server_data data;
    data.server = this;
    data.connection = sockfd;
    data.app = app;
    data.client = getpeer(sockfd);
    /* int tc = */ pthread_create(&thread, NULL, &lhs::server::client_thread_helper, (void*)&data);
  }

  quit();
  pthread_exit(NULL);
}

void lhs::server::quit() {
  if(socket_ > 0) {
    close(socket_);
  }
}

int lhs::server::port() {
  return port_;
}

const std::string lhs::server::address() {
  if(addr_.empty()) {
    return "0.0.0.0";
  }
  return addr_;
}

void *lhs::server::client_thread(void *context) {
  struct t_server_data *data = (struct t_server_data *)context;
  int c_socket = data->connection;
  struct peer client = data->client;
  lhs::http::middleware *app = data->app;

  std::vector<char> buffer;

  while(true) {
    std::vector<char> _buffer(BUFFER_SIZE);
    ssize_t read_size = read(c_socket, reinterpret_cast<char*>(&_buffer[0]), BUFFER_SIZE);

    if(0 > read_size) {
      THROW(server_error, strerror(errno));
    }
    if(0 == read_size) {
      break;
    }
    _buffer.resize(read_size);
    buffer.insert(buffer.end(), _buffer.begin(), _buffer.end());
    if(read_size < BUFFER_SIZE) {
      break;
    }
  } 

  lhs::http::request request(buffer);

#ifdef DEBUG
  std::cout << request << std::endl;
#endif

  // TODO -- replace by log
  std::cerr << lhs::util::format("%s:%d [%s] - - %s %s %s",
      client.ip,
      client.port,
      lhs::util::now_to_rfc1123().c_str(),
      lhs::http::get_http_method_name(request.method).c_str(),
      request.uri.c_str(),
      lhs::http::get_http_version_name(request.version).c_str()) << std::endl;

  lhs::http::response response(request.version);

  lhs::http::env env(request);

  bool internal_404 = true;

  if(app) {
    response = app->call(env);
    internal_404 = (response.code == 0);
  } 

  if(internal_404) {
    response.code = lhs::http::Not_Found;
    response.body(lhs::util::format(DEFAULT_NOT_FOUND, request.uri.c_str()));
    response["Content-Type"] = "text/html";
    response["Pragma"] = "no-cache";
  }

  if(-1 > response.write(c_socket)) {
    THROW(server_error, strerror(errno));
  }

  close(c_socket);
  pthread_exit(NULL);
}

struct lhs::server::peer lhs::server::getpeer(int socket) {
  socklen_t len;
  struct sockaddr_storage addr;
  peer p;

  len = sizeof addr;
  getpeername(socket, (struct sockaddr*)&addr, &len);

  // deal with both IPv4 and IPv6:
  if (addr.ss_family == AF_INET) {
    struct sockaddr_in *s = (struct sockaddr_in *)&addr;
    p.port = ntohs(s->sin_port);
    inet_ntop(AF_INET, &s->sin_addr, p.ip, sizeof(p.ip));
  } else { // AF_INET6
    struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
    p.port = ntohs(s->sin6_port);
    inet_ntop(AF_INET6, &s->sin6_addr, p.ip, sizeof(p.ip));
  }

  return p;
}

void *lhs::server::client_thread_helper(void *context) {
  return ((struct t_server_data *)context)->server->client_thread(context);
}

