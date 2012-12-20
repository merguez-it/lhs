#ifndef __HTTP_URI_H
#define __HTTP_URI_H

#include <string>

namespace lhs {
  namespace http {
    namespace uri {
      std::string decode(const std::string & sSrc);
      std::string encode(const std::string & sSrc);
    }
  }
}

#endif // __HTTP_URI_H

