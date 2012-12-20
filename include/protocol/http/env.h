#ifndef __HTTP_ENV_H
#define __HTTP_ENV_H

#include "protocol/http/request.h"
#include "protocol/http/response.h"

namespace lhs {
  namespace http {
    class env {
      public:
        env(lhs::http::request req);
        env(lhs::http::request req, lhs::http::response res);

      public:
        lhs::http::request request;
        lhs::http::response response;
    };
  }
}

#endif // __HTTP_ENV_H

