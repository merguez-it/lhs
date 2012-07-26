#include "http/env.h"

lhs::http::env::env(lhs::http::request req) : request(req) {
  response.version = request.version;
}

lhs::http::env::env(lhs::http::request req, lhs::http::response res) : request(req), response(res) {
  response.version = request.version;
}

