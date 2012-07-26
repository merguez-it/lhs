#ifndef __HTTP_H
#define __HTTP_H

#include <string>
#include "util/map.h"

#define LHS_VERSION "0.1"
#define LHS_HOST "LHS/0.1"
#define DEFAULT_NOT_FOUND \
  "<html>" \
  "  <head>" \
  "    <title>404 Not Found</title>" \
  "  </head>" \
  "  <body>" \
  "    <h1>Sorry dude!</h1>" \
  "    <p>The requested URL %s was not found on this server.</p>" \
  "  </body>" \
  " </html>"

#define HTTP_METHODS \
  __(UNKNOW, "UNKNOW", 0) \
  __(OPTION, "OPTION", 1) \
  __(GET, "GET", 2) \
  __(HEAD, "HEAD", 3) \
  __(POST, "POST", 4) \
  __(PUT, "PUT", 5) \
  __(DELETE, "DELETE", 6) \
  __(TRACE, "TRACE", 7) \
  __(CONNECT, "CONNECT", 8)

#define HTTP_VERSIONS \
  __(HTTP_UNKNOW, "HTTP/unknow", 0) \
  __(HTTP_1_0, "HTTP/1.0", 1) \
  __(HTTP_1_1, "HTTP/1.1", 2)

#define HTTP_ERRORS \
  __(ERROR_UNKNOW, "ERROR_UNKNOW", 0) \
  __(Continue, "Continue", 100) \
  __(SwitchingProtocols, "Switching Protocols", 101) \
  __(Processing, "Processing", 102) \
  __(Connection_timed_out, "Connection timed out", 118) \
  __(OK, "OK", 200) \
  __(Created, "Created", 201) \
  __(Accepted, "Accepted", 202) \
  __(NonAuthoritative_Information, "Non-Authoritative Information", 203) \
  __(No_Content, "No Content", 204) \
  __(Reset_Content, "Reset Content", 205) \
  __(Partial_Content, "Partial Content", 206) \
  __(MultiStatus, "Multi-Status", 207) \
  __(Content_Different, "Content Different", 210) \
  __(Multiple_Choices, "Multiple Choices", 300) \
  __(Moved_Permanently, "Moved Permanently", 301) \
  __(Moved_Temporarily, "Moved Temporarily", 302) \
  __(See_Other, "See Other", 303) \
  __(Not_Modified, "Not Modified", 304) \
  __(Use_Proxy, "Use Proxy", 305) \
  __(Temporary_Redirect, "Temporary Redirect", 307) \
  __(Too_many_Redirects, "Too many Redirects", 310) \
  __(Bad_Request, "Bad Request", 400) \
  __(Unauthorized, "Unauthorized", 401) \
  __(Payment_Required, "Payment Required", 402) \
  __(Forbidden, "Forbidden", 403) \
  __(Not_Found, "Not Found", 404) \
  __(Method_Not_Allowed, "Method Not Allowed", 405) \
  __(Not_Acceptable, "Not Acceptable", 406) \
  __(Proxy_Authentication_Required, "Proxy Authentication Required", 407) \
  __(Request_Timeout, "Request Time-out", 408) \
  __(Conflict, "Conflict", 409) \
  __(Gone, "Gone", 410) \
  __(Length_Required, "Length Required", 411) \
  __(Precondition_Failed, "Precondition Failed", 412) \
  __(Request_Entity_Too_Large, "Request Entity Too Large", 413) \
  __(RequestURI_Too_Long, "Request-URI Too Long", 414) \
  __(Unsupported_Media_Type, "Unsupported Media Type", 415) \
  __(Requested_range_unsatisfiable, "Requested range unsatisfiable", 416) \
  __(Expectation_failed, "Expectation failed", 417) \
  __(I_m_a_teapot, "I'm a teapot", 418) \
  __(Unprocessable_entity, "Unprocessable entity", 422) \
  __(Locked, "Locked", 423) \
  __(Method_failure, "Method failure", 424) \
  __(Unordered_Collection, "Unordered Collection", 425) \
  __(Upgrade_Required, "Upgrade Required", 426) \
  __(Retry_With, "Retry With", 449) \
  __(Blocked_by_Windows_Parental_Controls, "Blocked by Windows Parental Controls", 450) \
  __(Internal_Server_Error, "Internal Server Error", 500) \
  __(Not_Implemented, "Not Implemented", 501) \
  __(Bad_Gateway_ou_Proxy_Error, "Bad Gateway", 502) \
  __(Service_Unavailable, "Service Unavailable", 503) \
  __(Gateway_Timeout, "Gateway Time-out", 504) \
  __(HTTP_Version_not_supported, "HTTP Version not supported", 505) \
  __(Insufficient_storage, "Insufficient storage", 507) \
  __(Bandwidth_Limit_Exceeded, "Bandwidth Limit Exceeded", 509)

namespace lhs {
  namespace http {

#define __(name, str, code) name = code, 
    typedef enum {
      HTTP_METHODS
    } method;

    typedef enum {
      HTTP_VERSIONS
    } version;

    typedef enum {
      HTTP_ERRORS
    } error;
#undef __

#define __(name, str, code) (str, name)
    static std::map<std::string, method> method_str_to_value = 
      lhs::util::create_map<std::string, method>
      HTTP_METHODS;

    static std::map<std::string, version> version_str_to_value =
      lhs::util::create_map<std::string, version>
      HTTP_VERSIONS;

    static std::map<std::string, error> error_str_to_value =
      lhs::util::create_map<std::string, error>
      HTTP_ERRORS;
#undef __

#define __(name, str, code) (name, str)
    static std::map<method, std::string> method_value_to_str =
      lhs::util::create_map<method, std::string>
      HTTP_METHODS;

    static std::map<version, std::string> version_value_to_str =
      lhs::util::create_map<version, std::string>
      HTTP_VERSIONS;

    static std::map<error, std::string> error_value_to_str =
      lhs::util::create_map<error, std::string>
      HTTP_ERRORS;
#undef __

#define GET_BY_NAME_P(m) m get_http_ ## m ## _by_name(const std::string & name)
#define GET_NAME_P(m) std::string get_http_ ## m ## _name(m value)
    GET_BY_NAME_P(method);
    GET_NAME_P(method);

    GET_BY_NAME_P(version);
    GET_NAME_P(version);

    GET_BY_NAME_P(error);
    GET_NAME_P(error);
  }
}

#endif // __HTTP_H

