#ifndef __UTIL_DATETIME_H
#define __UTIL_DATETIME_H

#include <string>

#include <ctime>

namespace lhs {
  namespace util {
    std::string time_to_rfc1123( time_t time );
    std::string now_to_rfc1123();
  }
}

#endif // __UTIL_DATETIME_H
