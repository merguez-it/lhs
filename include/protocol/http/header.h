#ifndef __HTTP_HEADER_H
#define __HTTP_HEADER_H

#include "util/any.h"
#include <string>
#include <map>
#include <vector>

typedef enum HeadersFields {
  // Common
  Cache_Control,
  Connection,
  Content_Length,
  Content_MD5,
  Content_Type,
  Date,
  Pragma,
  Via,
  Warning,

  // Request only
  Accept,
  Accept_Charset,
  Accept_Datetime,
  Accept_Encoding,
  Accept_Language,
  Authorization,
  Cookie,
  Expect,
  From,
  Host,
  If_Match,
  If_Modified_Since,
  If_None_Match,
  If_Range,
  If_Unmodified_Since,
  Max_Forwards,
  Proxy_Authorization,
  Range,
  Referer,
  TE,
  Upgrade,
  User_Agent,

  // Response only
  Accept_Ranges,
  Age,
  Allow,
  Content_Encoding,
  Content_Language,
  Content_Location,
  Content_Disposition,
  Content_Range,
  ETag,
  Expires,
  Last_Modified,
  Link,
  Location,
  P3P,
  Proxy_Authenticate,
  Refresh,
  Retry_After,
  Server,
  Set_Cookie,
  Strict_Transport_Security,
  Trailer,
  Transfer_Encoding,
  Vary,
  WWW_Authenticate
} HeadersFields;

namespace lhs {
  namespace http {
    class header {
      public:
        header() {};
        mgz::util::any & operator[](const std::string & key);
        std::string to_string() const;

      private:
        std::map<std::string, mgz::util::any> datas_;
        std::vector<char> data_;
    };
  }
}

#endif // __HTTP_HEADER_H

