#ifndef __UTIL_RE_H
#define __UTIL_RE_H

#include "util/trex.h"
#include <string>
#include <vector>

namespace lhs {
  namespace util {
    class RegexException {};
    class RE {
      struct capture_t {
        size_t pos;
        size_t len;
        std::string capture;
        bool replaced;
      };

      public:
      RE(std::string pattern);
      ~RE();
      bool find(std::string data);
      size_t captures();
      std::string match(size_t i);
      std::string operator[](size_t i);
      std::string replace(size_t i, std::string repl);
      std::string replace(size_t i, size_t j);
      std::string replace_all(std::string repl);
      std::string replace_all(size_t j);
      std::string undo(size_t i);
      std::string undo_all();

      static std::string quote(std::string pattern);

      private:
      bool find_matchings(const TRexChar * text_begin, const TRexChar * text_end); 
      void repos(size_t pos, int corr);

      private:
      std::string pattern_;
      char * data_c_;
      std::string data_;
      TRex * preg_;
      std::vector<capture_t> captures_;
    };
  }
}

#endif // __UTIL_RE_H
