#ifndef __MIDDLEWARE_H
#define __MIDDLEWARE_H

#include "protocol/http/env.h"

namespace lhs {
  namespace http {
    class middleware {
      public:
        middleware(middleware *app);
        middleware();

        virtual ~middleware();

        virtual lhs::http::response call(lhs::http::env env) = 0;
        lhs::http::request get_request(lhs::http::env env);
        lhs::http::response get_response(lhs::http::env env);

      protected:
        middleware *app_;
    };
  }
}

#endif // __MIDDLEWARE_H
