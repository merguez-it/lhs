#include "middleware/urlmap.h"
#include "util/re.h"

bool sortmapping(lhs::middleware::mapping a, lhs::middleware::mapping b) {
  return (a.location.size() > b.location.size());
}

lhs::middleware::urlmap::urlmap(std::map<std::string, lhs::http::middleware *> maps) : middleware() {
  remap(maps);
}

lhs::middleware::urlmap::~urlmap() {
}

lhs::http::response lhs::middleware::urlmap::get_response(lhs::http::env env) {
  lhs::http::middleware *app = find_app(env.request);
  if(NULL != app) {
    return app->call(env);
  } else {
    return env.response;
  }
}

lhs::http::response lhs::middleware::urlmap::call(lhs::http::env env) {
  return get_response(env);
}

lhs::http::middleware * lhs::middleware::urlmap::find_app(lhs::http::request req) {
  std::vector<mapping>::iterator it;
  for(it = mapping_.begin(); it < mapping_.end(); it++) {
    if(lhs::util::RE((*it).match).find(req.uri)) {
      return (*it).app;
    }
  }
  return NULL;
}

void lhs::middleware::urlmap::remap(std::map<std::string, lhs::http::middleware *> maps) {
  std::map<std::string, middleware *>::iterator it;
  for(it = maps.begin(); it != maps.end(); it++) {
    struct lhs::middleware::mapping m;

    m.location = (*it).first;
    m.app = (*it).second;
    m.host = ""; 
    lhs::util::RE re("https?://(.*)(/.*)");
    if(re.find(m.location)) {
      m.host = re[0];
      m.location = re[1];
    } 
    m.match = "^" + lhs::util::replace_all(lhs::util::RE::quote(m.location), "/", "/+") + "(.*)";

    mapping_.push_back(m);
    std::sort(mapping_.begin(), mapping_.end(), sortmapping);
  }
}
