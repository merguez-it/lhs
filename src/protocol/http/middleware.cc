#include <cstdio>
#include "protocol/http/middleware.h"

lhs::http::middleware::middleware(middleware *app) : app_(app) {};
lhs::http::middleware::middleware() : app_(NULL) {};

lhs::http::middleware::~middleware() {
  if(app_) {
    delete app_;
  }
};

lhs::http::request lhs::http::middleware::get_request(lhs::http::env env) {
  return env.request;
}

lhs::http::response lhs::http::middleware::get_response(lhs::http::env env) {
  if(app_) {
    return app_->call(env);
  }

  return env.response;
}
