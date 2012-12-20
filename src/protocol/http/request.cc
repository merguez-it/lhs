#include "protocol/http/request.h"
#include "protocol/http/uri.h"
#include "util/string.h"
#include <sstream>
#include <iostream>
#include <deque>
#include <string.h>

lhs::http::request::request(const char *data) {
  data_ = std::vector<char>(data, data+strlen(data));
  parse();
}

lhs::http::request::request(const std::vector<char> & data) : data_(data) {
  parse();
}

lhs::http::header lhs::http::request::header() {
  return header_;
}

lhs::util::any & lhs::http::request::operator[](const std::string & key) {
  return header_[key];
}

void lhs::http::request::parse() {
  size_t nbchar = 0;
  std::string data_as_string(data_.begin(), data_.end());

  std::istringstream stream(data_as_string);
  std::string line;
  std::deque<std::string> result;
  while (std::getline( stream, line )) {
    if (line[line.size() - 1] == '\r') {
      line.resize(line.size() - 1);
    }
    result.push_back( line );
  }

  std::string init = result.front();
  parse_init(init);
  result.pop_front();
  nbchar = nbchar + init.size() + 1;

  while(!(line = result.front()).empty()) {
    size_t found = line.find_first_of(":");
    std::string header_name(line, 0, found);
    std::string header_value(line, found + 1);
    header_[lhs::util::trim(header_name)] = lhs::util::trim(header_value);

    nbchar = nbchar + line.size() + 1;
    result.pop_front();
  }

  nbchar = nbchar + 1; // The empty line
  body = std::vector<char>(data_.begin() + nbchar, data_.end());

  // TODO DECODE BODY
  //
  // Content-Type: application/x-www-form-urlencoded
  //   -> parse body to params
  //
  // Content-Type: multipart/form-data; boundary=----------------------------375e635b5861
  // Expect: 100-continue
  //   -> reread to get data and parse
}

void lhs::http::request::parse_init(const std::string & init) {
  std::vector<std::string> content = lhs::util::split(init, ' ');
  if(content.size() != 3) {
    THROW(request_error, "Bad request");
  }

  method = lhs::http::get_http_method_by_name(content[0]);
  uri = content[1];
  version = lhs::http::get_http_version_by_name(content[2]);

  parse_params();
  if(path[0] != '/') {
    THROW(request_error, "Invalide request path");
  }
}

void lhs::http::request::parse_params() {
  std::vector<std::string> content = lhs::util::split(uri, '?');
  if(content.size() < 2) {
    path = uri;
    query_string = "";
    return;
  }

  path = content[0];
  query_string = content[1];

  std::vector<std::string> _params = lhs::util::split(query_string, '&');
  std::vector<std::string>::iterator it;
  for(it = _params.begin(); it < _params.end(); it++) {
    size_t found = (*it).find_first_of("=");
    std::string param_name(*it, 0, found);
    std::string param_value(*it, found + 1);
    params[lhs::http::uri::decode(param_name)] = lhs::http::uri::decode(param_value);
  }
}

std::string lhs::http::request::to_string() const {
  std::string result("<lhs::http::request:\n");

  result += "<header: \n";
  result += header_.to_string() + "\n";
  result += ">\n";

  result += "<method: " + lhs::http::get_http_method_name(method) + " >\n";
  result += "<uri: " + uri + " >\n";
  result += "<query_string: " + query_string + " >\n";
  result += "<path: " + path + " >\n";
  result += "<version: " + lhs::http::get_http_version_name(version) + " >\n";

  result += ">";

  return result;
}

std::ostream& operator<< (std::ostream & out, const lhs::http::request & base) {
  out << base.to_string();
  return out;
}

