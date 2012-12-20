#ifndef __HANDLER_URLMAP_H
#define __HANDLER_URLMAP_H

#include "protocol/http/middleware.h"

namespace lhs {
  namespace middleware {
    struct mapping {
      std::string host;
      std::string location;
      std::string match;
      lhs::http::middleware *app;
    };

    class urlmap : public lhs::http::middleware {
      public:
        urlmap(std::map<std::string, lhs::http::middleware *> maps);
        ~urlmap();

        lhs::http::response get_response(lhs::http::env env);
        lhs::http::response call(lhs::http::env env);

      private:
        lhs::http::middleware * find_app(lhs::http::request req);
        void remap(std::map<std::string, lhs::http::middleware *> maps);

      private:
        std::vector<mapping> mapping_;
    };
  }
}
#endif // __HANDLER_URLMAP_H

