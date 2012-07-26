#include "util/datetime.h"

namespace lhs {
  namespace util {
    std::string time_to_rfc1123( time_t time ) {
      struct tm * timeinfo;
      char buffer [80];

      timeinfo = gmtime ( &time );
      strftime (buffer,80,"%a, %d %b %Y %H:%M:%S GMT",timeinfo);

      return buffer;
    }

    std::string now_to_rfc1123() {
      time_t now;
      time(&now);
      return time_to_rfc1123(now);
    }
  }
}
