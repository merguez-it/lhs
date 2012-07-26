#include "server.h"
#include "http/handler.h"
#include "handler/static_file.h"
#include "handler/error_404.h"
#include "util/exception.h"

#include <iostream>

#include <cstdio>
#include <signal.h>

lhs::server server;
lhs::http::handler *app = NULL;

void terminate(int sig) {
  std::cout << "Bye! Bye!" << std::endl;
  server.quit();
  if(NULL != app) {
    delete app;
  }
  exit(0);
}

int main() {
  signal(SIGTERM, terminate);
  signal(SIGKILL, terminate);
  signal(SIGINT, terminate);

  server.init();
  std::cout << "Serving HTTP on " << server.address() << " port " << server.port() << " ..." << std::endl;

  app = new lhs::handler::error_404();
  app = new lhs::handler::static_file(true, app);

  try {
    server.run(app);
  } catch(RuntimeException & e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  server.quit();
  delete app;
  return 0;
}
