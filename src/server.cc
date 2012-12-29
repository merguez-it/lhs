#include "config.h"
#include "server.h"
#include "protocol/http/env.h"
#include "protocol/http/request.h"
#include "protocol/http/response.h"
#include "util/string.h"
#include "util/datetime.h"

#include <vector>
#include <iostream>

#if !defined _WIN32 && !defined __WIN32__ && !defined WIN32
#include <arpa/inet.h>
#include <sys/socket.h>
#endif
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif
#include <cstdio>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_PORT 2009
#define MAX_CONNECTIONS 5
#define BUFFER_SIZE 1024

struct t_server_data {
#if defined _WIN32 || defined __WIN32__ || defined WIN32
  SOCKET connection;
#else
  int connection;
#endif
  struct lhs::peer client;
  lhs::server *server;
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
#if defined _WIN32 || defined __WIN32__ || defined WIN32
  if(0 != WSAStartup(MAKEWORD(2 ,0), &wsa_)) {
    THROW(server_error, "%s@%d: %s", __FILE__, __LINE__, strerror(errno));
  }
#endif

  sockaddr_.sin_family = AF_INET;
  sockaddr_.sin_port = ntohs(port_);
  if(addr_.empty()) {
    //sockaddr_.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr_.sin_addr.s_addr = INADDR_ANY;
  } else {
    //const char *addr_c = addr_.c_str();
    //inet_aton(addr_c, (in_addr*)&sockaddr_.sin_addr.s_addr);
    //inet_pton(sockaddr_.sin_family, addr_c, &sockaddr_.sin_addr);
    sockaddr_.sin_addr.s_addr = inet_addr(addr_.c_str());
  }
  memset(sockaddr_.sin_zero, 0, sizeof(sockaddr_.sin_zero));

  if(0 > (socket_ = ::socket(sockaddr_.sin_family, SOCK_STREAM, 0))) {
    THROW(server_error, "%s@%d: %s", __FILE__, __LINE__, strerror(errno));
  }

  int yes = 1; 
  setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes));

  int error;
  if(0 != (error = ::bind(socket_, (struct sockaddr*)&sockaddr_, sizeof(struct sockaddr)))) {
    THROW(server_error, "%s@%d - ERROR#%d : %s", __FILE__, __LINE__, error, strerror(errno));
  }

  if(0 != (error = ::listen(socket_, max_conn))) {
    THROW(server_error, "%s@%d - ERROR#%d : %s", __FILE__, __LINE__, error, strerror(errno));
  }
}

void lhs::server::run() {
  run(NULL);
}
void lhs::server::run(lhs::http::middleware *app) {
  while(true) {
    struct sockaddr_in client_ = {0};
#if defined _WIN32 || defined __WIN32__ || defined WIN32
    int max_size = sizeof(client_);
#else
    socklen_t max_size;
#endif
    int sockfd = ::accept(socket_, (struct sockaddr*)&client_, &max_size);
    if(0 > sockfd) {
      THROW(server_error, "%s@%d: %s", __FILE__, __LINE__, strerror(errno));
    }

    pthread_t thread;
    struct t_server_data data;
    data.server = this;
    data.connection = sockfd;
    data.app = app;
    data.client = getpeer(client_);
    /* int tc = */ pthread_create(&thread, NULL, &lhs::server::client_thread_helper, (void*)&data);
  }

  quit();
  pthread_exit(NULL);
}

void lhs::server::quit() {
  if(socket_ > 0) {
#if defined _WIN32 || defined __WIN32__ || defined WIN32
    closesocket(socket_);
    WSACleanup();
#else
    close(socket_);
#endif
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

#if defined _WIN32 || defined __WIN32__ || defined WIN32
#define socket_read(S, B, L) recv(S, B, L, 0);
#else
#define socket_read(S, B, L) read(S, B, L)
#endif
void *lhs::server::client_thread(void *context) {
  struct t_server_data *data = (struct t_server_data *)context;
#if defined _WIN32 || defined __WIN32__ || defined WIN32
  SOCKET c_socket = data->connection;
#else
  int c_socket = data->connection;
#endif
  struct lhs::peer client = data->client;
  lhs::http::middleware *app = data->app;

  std::vector<char> buffer;

  while(true) {
    std::vector<char> _buffer(BUFFER_SIZE);
    ssize_t read_size = socket_read(c_socket, reinterpret_cast<char*>(&_buffer[0]), BUFFER_SIZE);

    if(0 > read_size) {
      // TODO -- replace by log
      std::cerr << "Connexion reset by peer!" << std::endl;
      return NULL;
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
  std::cerr << mgz::util::format("%s:%d [%s] - - %s %s %s",
      client.ip,
      client.port,
      mgz::util::datetime::now().to_http().c_str(),
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
    response.body(mgz::util::format(DEFAULT_NOT_FOUND, request.uri.c_str()));
    response["Content-Type"] = "text/html";
    response["Pragma"] = "no-cache";
  }

  if(-1 > response.write(c_socket)) {
    THROW(server_error, "%s@%d: %s", __FILE__, __LINE__, strerror(errno));
  }

  close(c_socket);
  pthread_exit(NULL);
  return NULL;
}

struct lhs::peer lhs::server::getpeer(struct sockaddr_in &client) {
  lhs::peer p = {0};
  char *ip = inet_ntoa(client.sin_addr);
  memcpy(p.ip, ip, strlen(ip));
  p.port = ntohs(client.sin_port);

  return p;
}

void *lhs::server::client_thread_helper(void *context) {
  return ((struct t_server_data *)context)->server->client_thread(context);
}

