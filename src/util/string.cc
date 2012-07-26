#include "util/string.h"
#include <algorithm>

#include "util/varg.h"

namespace lhs {
  namespace util {
    std::string trim(std::string str) {
      std::string::size_type pos = str.find_last_not_of(' ');
      if(pos != std::string::npos) {
        str.erase(pos + 1);
        pos = str.find_first_not_of(' ');
        if(pos != std::string::npos) str.erase(0, pos);
      } else {
        str.erase(str.begin(), str.end());
      }
      return str;
    }

    std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems) {
      std::stringstream ss(s);
      std::string item;
      while(std::getline(ss, item, delim)) {
        elems.push_back(trim(item));
      }
      return elems;
    }

    std::vector<std::string> split(const std::string &s, char delim) {
      std::vector<std::string> elems;
      return split(s, delim, elems);
    }

    std::string join(std::vector<std::string> vector, char delim) {
      std::string result;
      std::vector<std::string>::iterator it;
      for (it = vector.begin() ; it < vector.end(); it++) {
        if(!result.empty()) {
          result.append(1, delim);
        }
        result.append(*it);
      }
      return result;
    }

    std::string replace_delim(const std::string &s, char i, char t) {
      return replace_all(s,std::string(1,i),std::string(1,t));
    }

    std::string to_upper(std::string s) {
      std::string r = s;
      std::transform(r.begin(), r.end(), r.begin(), ::toupper);
      return r;
    }

    std::string to_lower(std::string s) {
      std::string r = s;
      std::transform(r.begin(), r.end(), r.begin(), ::tolower);
      return r;
    }

    std::string classify(std::string s) {
      std::string r("");
      std::vector<std::string> l = split(s, '_');
      std::vector<std::string>::iterator it; 
      for(it = l.begin(); it < l.end(); it++) {
        std::string p(to_lower(*it));
        p[0] = toupper(p[0]);
        r += p;
      }
      return r;
    }

    std::string format(const std::string &fmt, ...) {
      VARARGS_TO_STRING(fmt, str);
      return str;
    }

    std::string random_string(const int len, const std::string prefix) {
      static bool init = false;
      if(!init) {
        srand(time(NULL));
        init = true;
      }
      std::string s(prefix);

      static const char alphanum[] = "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
      for (int i = 0; i < len; ++i) {
        s.append(1, alphanum[rand() % (sizeof(alphanum) - 1)]);
      }

      return s;
    }

    void replace_all(std::string& str, const std::string& from, const std::string& to) {
      size_t start_pos = 0;
      while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); 
      }
    }

    std::string replace_all(const std::string & str, const std::string& from, const std::string& to) {
      std::string out(str);
      replace_all(out, from, to);
      return out;
    }

    int asInt(const std::string &intString) {
      std::stringstream s;
      int result;
      s << intString;
      s >> result;
      return result;
    }

    int levenshtein_distance(const std::string source, const std::string target) {
      const int n = source.length();
      const int m = target.length();
      if (n == 0) {
        return m;
      }
      if (m == 0) {
        return n;
      }

      typedef std::vector< std::vector<int> > Tmatrix; 
      Tmatrix matrix(n+1);

      for (int i = 0; i <= n; i++) {
        matrix[i].resize(m+1);
      }

      for (int i = 0; i <= n; i++) {
        matrix[i][0]=i;
      }

      for (int j = 0; j <= m; j++) {
        matrix[0][j]=j;
      }

      for (int i = 1; i <= n; i++) {
        const char s_i = source[i-1];

        for (int j = 1; j <= m; j++) {
          const char t_j = target[j-1];

          int cost;
          if (s_i == t_j) {
            cost = 0;
          } else {
            cost = 1;
          }

          const int above = matrix[i-1][j];
          const int left = matrix[i][j-1];
          const int diag = matrix[i-1][j-1];
          int cell = std::min( above + 1, std::min(left + 1, diag + cost));

          if (i>2 && j>2) {
            int trans = matrix[i-2][j-2]+1;
            if (source[i-2] != t_j) trans++;
            if (s_i != target[j-2]) trans++;
            if (cell > trans) cell = trans;
          }

          matrix[i][j] = cell;
        }
      }

      return matrix[n][m];
    }

    static char lookup_en[] = {
      '0',    /* A */
      '1',    /* B */
      '2',    /* C */
      '3',    /* D */
      '0',    /* E */
      '1',    /* F */
      '2',    /* G */
      '0',    /* H */
      '0',    /* I */
      '2',    /* J */
      '2',    /* K */
      '4',    /* L */
      '5',    /* M */
      '5',    /* N */
      '0',    /* O */
      '1',    /* P */
      '0',    /* Q */
      '6',    /* R */
      '2',    /* S */
      '3',    /* T */
      '0',    /* U */
      '1',    /* V */
      '0',    /* W */
      '2',    /* X */
      '0',    /* Y */
      '2',    /* Z */
    };

    static char lookup_fr[] = {
      '0',    /* A */
      '1',    /* B */
      '2',    /* C */
      '3',    /* D */
      '0',    /* E */
      '9',    /* F */
      '7',    /* G */
      '0',    /* H */
      '0',    /* I */
      '7',    /* J */
      '2',    /* K */
      '4',    /* L */
      '5',    /* M */
      '5',    /* N */
      '0',    /* O */
      '1',    /* P */
      '2',    /* Q */
      '6',    /* R */
      '8',    /* S */
      '3',    /* T */
      '0',    /* U */
      '9',    /* V */
      '0',    /* W */
      '8',    /* X */
      '0',    /* Y */
      '8',    /* Z */
    };
 
    std::string soundex(enum soundex_lang lang, const std::string & input, const size_t resultLength) {
      char *lookup;

      switch(lang) {
        case FR:
          lookup = lookup_fr;
          break;
        default:
          lookup = lookup_en;
      }

      std::string result = input.substr(0,1);

      for(size_t i=1; i < input.length(); i++){
        if(!isalpha(input[i])){
          continue;
        }

        const char lookupInput = islower(input[i]) ? toupper(input[i]) : input[i];
        const char *lookupVal = &lookup[lookupInput-'A'];

        if(result.find(lookupVal, 0) != 0 ){
          result.append(lookupVal);
        }
      }

      if(result.length() >= resultLength){
        return result.substr(0,resultLength-1);
      }

      return "Z000";
    }
  }
}
