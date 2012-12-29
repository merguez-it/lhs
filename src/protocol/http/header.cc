#include "protocol/http/header.h"

mgz::util::any & lhs::http::header::operator[](const std::string & key) {
  return datas_[key];
}

std::string lhs::http::header::to_string() const {
  std::string result("");
  std::map<std::string, mgz::util::any>::const_iterator it;

  for(it = datas_.begin(); it != datas_.end(); it++) {
    result += (*it).first + ": " + mgz::util::any_cast<std::string>((*it).second) + "\r\n";
  }

  return result;
}

