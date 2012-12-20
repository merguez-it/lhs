#ifndef __HTTP_REQUEST_H
#define __HTTP_REQUEST_H

#include "protocol/http.h"
#include "protocol/http/header.h"
#include "util/exception.h"

namespace lhs {
  namespace http {
    class request_error {};
    class request {
      public:
        request(const char *data);
        request(const std::vector<char> & data);

        lhs::http::header header();
        lhs::util::any & operator[](const std::string & key);

        std::string to_string() const;

      private:
        void parse();
        void parse_init(const std::string & init);
        void parse_params();

      public:
        lhs::http::method method;
        std::string uri;
        std::string query_string; 
        std::string path;
        lhs::http::version version;
        std::map<std::string, std::string> params;
        std::vector<char> body;

      private:
        std::vector<char> data_;
        lhs::http::header header_;
    };
  }
}

std::ostream& operator<< (std::ostream& out, const lhs::http::request & base);

#endif // __HTTP_REQUEST_H

