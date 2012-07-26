#ifndef __HANDLER_H
#define __HANDLER_H

#include "http/env.h"

namespace lhs {
  namespace http {
    class handler {
      public:
        handler(handler *app);
        handler();

        virtual ~handler();

        virtual lhs::http::response call(lhs::http::env env) = 0;
        lhs::http::request get_request(lhs::http::env env);
        lhs::http::response get_response(lhs::http::env env);

      protected:
        handler *app_;
    };
  }
}

#endif // __HANDLER_H
