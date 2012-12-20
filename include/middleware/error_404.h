#ifndef __HANDLER_ERROR_404_H
#define __HANDLER_ERROR_404_H

#include "protocol/http/middleware.h"

namespace lhs {
  namespace middleware {
    class error_404 : public lhs::http::middleware {
      public:
        error_404(middleware *app) : middleware(app) {}
        error_404() : middleware() {}

        lhs::http::response call(lhs::http::env env);
    };
  }
}

#endif // __HANDLER_ERROR_404_H

