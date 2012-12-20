#include "config.h"
#include "middleware/static_file.h"
#include "protocol/http/mime.h"
#include "util/string.h"

#include <fstream>
#include <iostream>
#include <algorithm>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#if defined _WIN32 || defined __WIN32__ || defined WIN32
#include <windows.h>
#define FILE_SEPARATOR "\\"
#define FILE_SEPARATOR_C '\\'
#else
#define FILE_SEPARATOR "/"
#define FILE_SEPARATOR_C '/'
#endif

lhs::middleware::static_file::static_file(bool allow, std::string root) : middleware(), allow_explore_(allow), root_(root) {
  set_root();
}
lhs::middleware::static_file::static_file(bool allow, std::string root, middleware *app) : middleware(app), allow_explore_(allow), root_(root) {
  set_root();
}
lhs::middleware::static_file::static_file(bool allow, middleware *app) : middleware(app), allow_explore_(allow), root_(".") {
  set_root();
}
lhs::middleware::static_file::static_file(bool allow) : middleware(), allow_explore_(allow), root_(".") {
  set_root();
}
lhs::middleware::static_file::static_file(std::string root) : middleware(), allow_explore_(false), root_(root) {
  set_root();
}
lhs::middleware::static_file::static_file(std::string root, middleware *app) : middleware(app), allow_explore_(false), root_(root) {
  set_root();
}
lhs::middleware::static_file::static_file(middleware *app) : middleware(app), allow_explore_(false), root_(".") {
  set_root();
}
lhs::middleware::static_file::static_file() : middleware(), allow_explore_(false), root_(".") {
  set_root();
}

lhs::http::response lhs::middleware::static_file::call(lhs::http::env env) {
  lhs::http::response response = get_response(env);
  lhs::http::request request = get_request(env);

  if(response.code == 0 || response.code == 404) {
    std::string filename = get_filename(std::string(request.path, 1));

    if(exist(filename)) {
      if(is_directory(filename)) {
        if(allow_explore_) {
          response["Content-Type"] = "text/html";
          response.code = lhs::http::OK;
          response.body(get_directory_content(filename));
        }
      } else {
        response["Content-Type"] = get_mime(filename);
        response.code = lhs::http::OK;
        response.body(get_file_content(filename));
      }
    }
  }

  return response;
}

std::string lhs::middleware::static_file::get_filename(const std::string & file) {
  std::string out = root_ + FILE_SEPARATOR + file;
  std::replace(out.begin(), out.end(), '/', FILE_SEPARATOR_C);
  if(out[out.length()-1] == FILE_SEPARATOR_C) {
    out = std::string(out.begin(), out.end() - 1);
  }
  return out;
}


void lhs::middleware::static_file::set_root() {
  if(root_.size() > 1 && root_[root_.length()-1] == FILE_SEPARATOR_C) {
    root_ = std::string(root_.begin(), root_.end()-1);
  }
}

bool lhs::middleware::static_file::exist(const std::string & filename) {
  bool result = !filename.empty();

  if(filename.find("..") != std::string::npos) {
    return false;
  }

  if(result) {
#ifdef HAVE_GETFILEATTRIBUTES
    result = result && (::GetFileAttributes(filename.c_str()) != 0xFFFFFFFF);
#else
    result = result && (-1 != access(filename.c_str(), 0));
#endif
  }
  return result;
}

bool lhs::middleware::static_file::is_directory(const std::string & filename) {
  struct stat status;
  if(0 != stat(filename.c_str(), &status)) {
    return false;
  }

  if (exist(filename) && (status.st_mode & S_IFMT) == S_IFDIR) {
    return true;
  } else {
    return false;
  }
}

std::string lhs::middleware::static_file::get_mime(const std::string & file) {
  std::string ext = file.substr(file.find_last_of(".") + 1);
  lhs::http::mime_type mime(ext);
  return mime[0].name;
}

#define HANDLER_STATIC_FILE_BUFFER_SIZE 1024*8
std::string lhs::middleware::static_file::get_file_content(const std::string & file) {

  std::ifstream io(file.c_str(), std::ios::in | std::ios::binary);

  std::vector<char> buffer;

  while(true) {
    std::vector<char> _buffer(HANDLER_STATIC_FILE_BUFFER_SIZE);
    io.read(reinterpret_cast<char*>(&_buffer[0]), HANDLER_STATIC_FILE_BUFFER_SIZE);
    int read_size = io.gcount();

    if(0 > read_size) {
      THROW(static_file_error, "Error while reading file %s", file.c_str());
    }
    if(0 == read_size) {
      break;
    }
    _buffer.resize(read_size);
    buffer.insert(buffer.end(), _buffer.begin(), _buffer.end());
    if(read_size < HANDLER_STATIC_FILE_BUFFER_SIZE) {
      break;
    }
  } 

  return std::string (buffer.begin(), buffer.end());
}

#define INDEX_HEADER \
  "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">" \
  "<html>" \
  "  <head>" \
  "    <title>Index of %s</title>" \
  "  </head>" \
  "  <body>" \
  "    <h1>Index of %s</h1>" \
  "    <table border=\"0\" cellspacing=\"10px\">" \
  "      <tr><th>&nbsp;</th><th>Name</th><th>Last modified</th><th>Size</th></tr><tr><th colspan=\"4\"><hr></th></tr>"

#define INDEX_FILE \
  "      <tr><td><img src=\"%s\" /></td><td><a href=\"%s\">%s</a></td><td>%s</td><td>%s</td></tr>"

#define INDEX_FOOTER \
  "      <tr><th colspan=\"4\"><hr></th></tr>" \
  "      <tr><td colspan=\"4\"><small>Powered by " LHS_HOST " Server</small></td></tr>" \
  "    </table>" \
  "  </body>" \
  "</html>"

#define FOLDER_IMAGE "data:image/png;base64," \
 "iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABGdBTUEAAK/INwWK" \
 "6QAAABl0RVh0U29mdHdhcmUAQWRvYmUgSW1hZ2VSZWFkeXHJZTwAAAGrSURBVDjL" \
 "xZO7ihRBFIa/6u0ZW7GHBUV0UQQTZzd3QdhMQxOfwMRXEANBMNQX0MzAzFAwEzHw" \
 "ARbNFDdwEd31Mj3X7a6uOr9BtzNjYjKBJ6nicP7v3KqcJFaxhBVtZUAK8OHlld2s" \
 "t7Xl3DJPVONP+zEUV4HqL5UDYHr5xvuQAjgl/Qs7TzvOOVAjxjlC+ePSwe6DfbVe" \
 "gLVuT4r14eTr6zvA8xSAoBLzx6pvj4l+DZIezuVkG9fY2H7YRQIMZIBwycmzH1/s" \
 "3F8AapfIPNF3kQk7+kw9PWBy+IZOdg5Ug3mkAATy/t0usovzGeCUWTjCz0B+Sj0e" \
 "kfdvkZ3abBv+U4GaCtJ1iEm6ANQJ6fEzrG/engcKw/wXQvEKxSEKQxRGKE7Izt+D" \
 "SiwBJMUSm71rguMYhQKrBygOIRStf4TiFFRBvbRGKiQLWP29yRSHKBTtfdBmHs0B" \
 "UpgvtgF4yRFR+NUKi0XZcYjCeCG2smkzLAHkbRBmP0/Uk26O5YnUActBp1GsAI+S" \
 "5nRJJJal5K1aAMrq0d6Tm9uI6zjyf75dAe6tx/SsWeD//o2/Ab6IH3/h25pOAAAA" \
 "AElFTkSuQmCC" 

#define FILE_IMAGE "data:image/png;base64," \
 "iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAQAAAC1+jfqAAAABGdBTUEAAK/INwWK" \
 "6QAAABl0RVh0U29mdHdhcmUAQWRvYmUgSW1hZ2VSZWFkeXHJZTwAAADoSURBVBgZ" \
 "BcExblNBGAbA2ceegTRBuIKOgiihSZNTcC5LUHAihNJR0kGKCDcYJY6D3/77MdOi" \
 "nTvzAgCw8ysThIvn/VojIyMjIyPP+bS1sUQIV2s95pBDDvmbP/mdkft83tpYguZq" \
 "5Jh/OeaYh+yzy8hTHvNlaxNNczm+la9OTlar1UdA/+C2A4trRCnD3jS8BB1obq2G" \
 "k6GU6QbQAS4BUaYSQAf4bhhKKTFdAzrAOwAxEUAH+KEM01SY3gM6wBsEAQB0gJ+m" \
 "aZoC3gI6iPYaAIBJsiRmHU0AALOeFC3aK2cWAACUXe7+AwO0lc9eTHYTAAAAAElF" \
 "TkSuQmCC"

std::string lhs::middleware::static_file::get_directory_content(const std::string & file) {
  std::string path(file.begin() + root_.size(), file.end());
  std::string result = lhs::util::format(INDEX_HEADER, path.c_str(), path.c_str());

  if(file != root_) {
    size_t found = path.find_last_of(FILE_SEPARATOR_C);

    std::string parent("/");
    if(found > 0) {
      parent = std::string(path.begin(), path.begin() + found);
      std::replace(parent.begin(), parent.end(), FILE_SEPARATOR_C, '/');
    }

    result += lhs::util::format(INDEX_FILE, FOLDER_IMAGE, parent.c_str(), "..", "-", "-");
  }

  DIR *dp;
  struct dirent *dirp;
  if((dp  = opendir(file.c_str())) == NULL) {
    return "";
  }

  while ((dirp = readdir(dp)) != NULL) {
    std::string local_path(dirp->d_name);
    if(0 != local_path.compare(".") && 0 != local_path.compare("..")) {
      std::string url = path + "/" + local_path;
      std::string sys_file = file + FILE_SEPARATOR + local_path;

      struct stat status;
      stat(sys_file.c_str(), &status);

      char modif_date[80] = {0};
      strftime(modif_date, 80, "%d-%b-%Y %H:%M", gmtime(&(status.st_mtime)));

      std::string type = FOLDER_IMAGE;
      std::string size = "-";
      if(!is_directory(sys_file)) {
        size = lhs::util::format("%.1fk", ((float)status.st_size)/1024.0); 
        type = FILE_IMAGE;
      }
      result += lhs::util::format(INDEX_FILE, type.c_str(), url.c_str(), local_path.c_str(), modif_date, size.c_str());
    }
  }

  result += INDEX_FOOTER;
  return result;
}

