#include "middleware/error_404.h"
#include "util/string.h"

#include <iostream>

lhs::http::response lhs::middleware::error_404::call(lhs::http::env env) {
  lhs::http::response response = get_response(env);
  lhs::http::request request = get_request(env);

  if(response.code == 0) {
    response.code = lhs::http::Not_Found;
    response.body(lhs::util::format(DEFAULT_NOT_FOUND, request.uri.c_str()));
    response["Content-Type"] = "text/html";
    response["Pragma"] = "no-cache";
  }

  return response;
}
