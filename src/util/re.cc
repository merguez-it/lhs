#include "util/re.h"
#include "util/string.h"
#include "util/exception.h"

#include <cstring>
#include <cstdlib>

namespace lhs {
  namespace util {
    RE::RE(std::string pattern) : pattern_(pattern) {
      data_c_ = NULL;
      const TRexChar *error = NULL;
      preg_ = trex_compile(pattern.c_str(), &error);
      if(!preg_) {
        THROW(RegexException, lhs::util::format("Regex compilation failed : %s", error));
      }
    }

    RE::~RE() {
      trex_free(preg_);
      if(data_c_) {
        free(data_c_);
      }
    }

    bool RE::find(std::string data) {
      data_ = data;
      data_c_ = (char*)malloc(sizeof(char)*(data_.length() + 1));
      std::memset(data_c_, 0, data_.length() + 1);
      std::strncpy(data_c_, data_.c_str(), data_.length());

      captures_.clear();

      const TRexChar * text_begin = data_c_;
      const TRexChar * text_end = data_c_ + std::strlen(data_c_);

      return find_matchings(text_begin, text_end);
    }

    bool RE::find_matchings(const TRexChar * text_begin, const TRexChar * text_end) {
      bool found = false;
      const TRexChar *begin, *end;

      if(trex_searchrange(preg_, text_begin, text_end, &begin, &end)) {
        found = true;
        int matchs = trex_getsubexpcount(preg_);
        if(matchs > 1) {
          for(int i = 1; i < matchs; i++) {
            TRexMatch match;
            capture_t mt;

            trex_getsubexp(preg_, i, &match);
            char *buff = (char*)malloc(sizeof(char)*(match.len + 1));
            std::memset(buff, 0, match.len + 1);
            std::strncpy(buff, match.begin, match.len);

            mt.capture = std::string(buff);
            mt.pos = match.begin - data_c_;
            mt.len = match.len;
            mt.replaced = false;
            captures_.push_back(mt);

            free(buff);
          }
        }

        TRexMatch match;
        trex_getsubexp(preg_, 0, &match);
        const TRexChar * new_begin = match.begin + match.len;
        if(new_begin != text_end) {
          find_matchings(new_begin, text_end);
        }
      }

      return found;
    }

    size_t RE::captures() {
      return captures_.size();
    }

    std::string RE::match(size_t i) {
      if(i < captures_.size()) {
        return captures_[i].capture;
      }

      return "";
    }

    std::string RE::operator[](size_t i) {
      return match(i);
    }

    std::string RE::replace(size_t i, std::string repl) {
      if(i >= captures()) {
        return data_;
      }

      std::string pre = data_.substr(0, captures_[i].pos);
      std::string post = data_.substr(captures_[i].pos + captures_[i].len);

      repos(i+1, repl.length() - captures_[i].len);
      captures_[i].len = repl.length();
      captures_[i].replaced = (captures_[i].capture != repl);

      data_ = pre + repl + post;
      return data_;
    }

    void RE::repos(size_t pos, int corr) {
      for(size_t i = pos; i < captures_.size(); i++) {
        captures_[i].pos += corr;
      }
    }

    std::string RE::replace(size_t i, size_t j) {
      if(j >= captures()) {
        return data_;
      }

      return replace(i, captures_[j].capture);
    }

    std::string RE::replace_all(std::string repl) {
      for(size_t i = 0; i < captures(); i++) {
        replace(i, repl);
      }

      return data_;
    }

    std::string RE::replace_all(size_t j) {
      if(j >= captures()) {
        return data_;
      }

      for(size_t i = 0; i < captures(); i++) {
        replace(i, j);
      }

      return data_;
    }

    std::string RE::undo(size_t i) {
      if(i < captures() && captures_[i].replaced) {
        return replace(i, captures_[i].capture);
      }
      return data_;
    }

    std::string RE::undo_all() {
      for(size_t i = 0; i < captures(); i++) {
        undo(i);
      }

      return data_;
    }

    std::string RE::quote(std::string pattern) {
      std::string result("");
      std::string::iterator it;
      for(it = pattern.begin(); it < pattern.end(); it++) {
        switch((char)(*it)) {
          case '[': case ']': case '{': case '}':
          case '(': case ')': case '|': case '-':
          case '*': case '.': case '\\':
          case '?': case '+': case '^': case '$':
          case ' ': case '#':
            result += '\\';
            result += *it;
            break;
          case '\t': 
            result += "\\t";
            break;
          case '\f': 
            result += "\\f";
            break;
          case '\v': 
            result += "\\v";
            break;
          case '\n': 
            result += "\\n";
            break;
          case '\r':
            result += "\\r";
            break;
          default:
            result += *it;
        };
      }

      return result;
    }
  }
}
