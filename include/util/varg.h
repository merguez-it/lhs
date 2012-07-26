#ifndef __VASPRINTF_GLOW_INCLUDE
#define __VASPRINTF_GLOW_INCLUDE

#include <cstdio>
#include <cstdarg>
#include <cstdlib>

#include "config.h"
#ifndef HAVE_VASPRINTF
#ifndef HAVE_VSNPRINTF
#pragma error "vasprintf and vsnprintf not present..."
#endif // HAVE_VSNPRINTF
int vasprintf( char **sptr, const char *fmt, va_list argv );
#endif // HAVE_VASPRINTF

#define VARARGS_TO_STRING(FORMAT, STRING) \
  char *ret; va_list ap; \
  va_start(ap, FORMAT); \
  vasprintf(&ret, FORMAT.c_str(), ap); \
  va_end(ap); \
  std::string STRING(ret);\
  free(ret);

#endif // __VASPRINTF_GLOW_INCLUDE

