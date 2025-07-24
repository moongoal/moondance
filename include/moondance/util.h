/**
 * String utilities.
 */
#ifndef MD_UTIL_H
#define MD_UTIL_H

#include <string.h>

#ifndef _WIN32
  #define strcpy_s(output, length, value) strcpy(output, value)
#endif // _WIN32

#define MD_QUOTE(x) MD_QUOTE_(x)
#define MD_QUOTE_(x) #x

#endif // MD_UTIL_H

