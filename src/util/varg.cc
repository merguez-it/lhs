#include "util/varg.h"

#ifndef HAVE_VASPRINTF
int vasprintf( char **sptr, const char *fmt, va_list argv ) { 
  int wanted = vsnprintf( *sptr = NULL, 0, fmt, argv ); 
  if( (wanted > 0) && ((*sptr = (char*)malloc( 1 + wanted )) != NULL) ) {
    return vsprintf( *sptr, fmt, argv ); 
  }

  return wanted; 
}
#endif // HAVE_VASPRINTF

