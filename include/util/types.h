#ifndef __UTIL_TYPES_H
#define __UTIL_TYPES_H

#include <sstream>

namespace lhs {
  namespace util {
    template <typename T> inline bool value_of(std::string & data, T & obj) {
      std::istringstream is(data);
      return is >> obj;
    }
    template <typename T> inline bool value_of(std::string & data, std::string & obj) {
      obj = data;
      return true;
    }
    template <typename T> inline T from_string(std::string & data) {
      T i;
      if(value_of<T>(data, i)) {
        return i;
      }
      return 0;
    }
    template <typename T> inline std::string to_string(T & data) {
      std::ostringstream os;
      os << data;
      return os.str();
    }

    template <typename F, typename T> T convert(F data) {
      std::string tmp = to_string<F>(data);
      return from_string<T>(tmp);
    }
  }
}

#endif // __UTIL_TYPES_H

