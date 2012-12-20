#include "protocol/http/response.h"
#include "util/datetime.h"
#include "util/string.h"
#include "util/types.h"
#include <unistd.h>

lhs::http::response::response() {};

lhs::http::response::response(lhs::http::version v) : version(v) {}

#if defined _WIN32 || defined __WIN32__ || defined WIN32
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#define socket_write(S, B, L) send(S, B, L, 0);
#else
#define socket_write(S, B, L) ::write(S, B, L)
#endif
ssize_t lhs::http::response::write(int socket) {
  std::vector<char> data;

  header_["Server"] = LHS_HOST;
  header_["Date"] = lhs::util::now_to_rfc1123();
  header_["Content-Length"] = lhs::util::convert<size_t, std::string>(body_.size());

  std::string header = lhs::util::format("%s %d %s\r\n", 
      lhs::http::get_http_version_name(version).c_str(),
      code,
      lhs::http::get_http_error_name(code).c_str());
  header += header_.to_string();
  header += "\r\n";

  data = std::vector<char>(header.begin(), header.end());
  data.insert(data.end(), body_.begin(), body_.end());

  return socket_write(socket, reinterpret_cast<char *>(&data[0]), data.size());
}

lhs::http::header lhs::http::response::header() {
  return header_;
}

lhs::util::any & lhs::http::response::operator[](const std::string & key) {
  return header_[key];
}

void lhs::http::response::body(std::vector<char> data) {
  body_ = data;
}

void lhs::http::response::body(std::string data) {
  body_ = std::vector<char>(data.begin(), data.end());
}

