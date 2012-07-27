#include "server.h"
#include "http/middleware.h"
#include "middleware/static_file.h"
#include "middleware/error_404.h"
#include "middleware/urlmap.h"
#include "util/exception.h"

#include <iostream>

#include <cstdio>
#include <signal.h>

class about : public lhs::http::middleware {
  public:
    about(lhs::http::middleware *app) : middleware(app) {}
    about() : middleware() {}

    lhs::http::response call(lhs::http::env env) {
      lhs::http::response r = get_response(env);

      r.body("ABOUT!!!");
      r.code = lhs::http::OK;

      r["Content-Type"] = "text/plain";

      return r;
    }
};

lhs::server server;
lhs::http::middleware *app = NULL;

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

  std::map<std::string, lhs::http::middleware *> maps;
  maps["/about"] = new about();
  app = new lhs::middleware::urlmap(maps);

  app = new lhs::middleware::error_404(app);
  app = new lhs::middleware::static_file(true, app);

  try {
    server.run(app);
  } catch(RuntimeException & e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  server.quit();
  delete app;
  return 0;
}
