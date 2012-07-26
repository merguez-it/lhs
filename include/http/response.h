#ifndef __HTTP_RESPONSE_H
#define __HTTP_RESPONSE_H

#include "http.h"
#include "http/header.h"

namespace lhs {
  namespace http {
    class response {

      public: 
        response();
        response(lhs::http::version http_version);

        ssize_t write(int socket);

        lhs::http::header header();
        lhs::util::any & operator[](const std::string & key);

        void body(std::vector<char> data);
        void body(std::string data);

      public:
        lhs::http::error code;
        lhs::http::version version;

      private:
        std::vector<char> body_;
        lhs::http::header header_;
    };
  }
}

#endif // __HTTP_RESPONSE_H

