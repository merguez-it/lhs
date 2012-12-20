#include "protocol/http.h"

#define GET_BY_NAME_I(m, d) \
  GET_BY_NAME_P(m) { \
    if(m ## _str_to_value.find(name) == m ## _str_to_value.end()) { \
      return d; \
    } \
    return m ## _str_to_value[name]; \
  }

#define GET_NAME_I(m) \
  GET_NAME_P(m) { \
    if(m ## _value_to_str.find(value) == m ## _value_to_str.end()) { \
      return ""; \
    } \
    return m ## _value_to_str[value]; \
  }
    
namespace lhs {
  namespace http {
    GET_BY_NAME_I(method, UNKNOW)
    GET_NAME_I(method)

    GET_BY_NAME_I(version, HTTP_UNKNOW)
    GET_NAME_I(version)

    GET_BY_NAME_I(error, Internal_Server_Error)
    GET_NAME_I(error)
  }
}
