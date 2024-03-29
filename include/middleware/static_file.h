#ifndef __HANDLER_STATIC_FILE_H
#define __HANDLER_STATIC_FILE_H

#include "protocol/http/middleware.h"
#include "util/exception.h"

namespace lhs {
  namespace middleware {
    class static_file_error {};
    class static_file : public lhs::http::middleware {
      public:
        static_file(bool allow, std::string root);
        static_file(bool allow, std::string root, middleware *app);
        static_file(bool allow, middleware *app);
        static_file(bool allow);

        static_file(std::string root);
        static_file(std::string root, middleware *app);
        static_file(middleware *app);
        static_file();

        lhs::http::response call(lhs::http::env env);

      private:
        void set_root();
        bool exist(const std::string & filename);
        bool is_directory(const std::string & filename);
        std::string get_filename(const std::string & file);
        std::string get_mime(const std::string & file);
        std::string get_file_content(const std::string & file);
        std::string get_directory_content(const std::string & file);

      private:
        bool allow_explore_;
        std::string root_;
    };
  }
}

#endif // __STATIC_FILE_H

