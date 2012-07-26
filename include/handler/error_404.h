#ifndef __HANDLER_ERROR_404_H
#define __HANDLER_ERROR_404_H

#include "http/handler.h"

namespace lhs {
  namespace handler {
    class error_404 : public lhs::http::handler {
      public:
        error_404(handler *app) : handler(app) {}
        error_404() : handler() {}

        lhs::http::response call(lhs::http::env env);
    };
  }
}

#endif // __HANDLER_ERROR_404_H

