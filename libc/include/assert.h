#ifndef __LIBC__ASSERT_H__
#define __LIBC__ASSERT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdlib.h"

#ifdef NDEBUG
#  define assert(condition) ((void)0)
#else
/* TODO: actually print something nice on assert failure */
/* NOLINTNEXTLINE(readability-identifier-naming) */
#  define assert(condition)                                                            \
      do {                                                                             \
          if (!(condition))                                                            \
              abort();                                                                 \
      } while (0)
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
