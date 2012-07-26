#include "http/header.h"

lhs::util::any & lhs::http::header::operator[](const std::string & key) {
  return datas_[key];
}

std::string lhs::http::header::to_string() {
  std::string result("");
  std::map<std::string, lhs::util::any>::iterator it;

  for(it = datas_.begin(); it != datas_.end(); it++) {
    result += (*it).first + ": " + lhs::util::any_cast<std::string>((*it).second) + "\r\n";
  }

  return result;
}

