#ifndef __UTIL_STRING_H
#define __UTIL_STRING_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>

namespace lhs {
  namespace util {
    std::string trim(std::string str);
    std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
    std::vector<std::string> split(const std::string &s, char delim);
    std::string join(std::vector<std::string> vector, char delim);
    std::string replace_delim(const std::string &s, char i, char t);
    std::string to_upper(std::string s); 
    std::string to_lower(std::string s); 
    std::string classify(std::string s); 
    std::string format(const std::string & format, ...);
    std::string random_string(const int len, const std::string prefix = ""); 
    void replace_all(std::string& str, const std::string& from, const std::string& to);
    std::string replace_all(const std::string & str, const std::string& from, const std::string& to);
    int asInt(const std::string &intString);
    int levenshtein_distance(const std::string source, const std::string target);

    enum soundex_lang { 
      FR,
      EN
    };
    std::string soundex(enum soundex_lang lang, const std::string & input, const int resultLength);

    class line {
      std::string data;
      public:
        friend std::istream &operator>>(std::istream &is, line &l) {
          std::getline(is, l.data);
          return is;
        }

        operator std::string() const { return data; }    
    };
  }
}

#endif
