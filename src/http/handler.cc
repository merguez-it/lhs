#include <cstdio>
#include "http/handler.h"

lhs::http::handler::handler(handler *app) : app_(app) {};
lhs::http::handler::handler() : app_(NULL) {};

lhs::http::handler::~handler() {
  if(app_) {
    delete app_;
  }
};

lhs::http::request lhs::http::handler::get_request(lhs::http::env env) {
  return env.request;
}

lhs::http::response lhs::http::handler::get_response(lhs::http::env env) {
  if(app_) {
    return app_->call(env);
  }

  return env.response;
}
