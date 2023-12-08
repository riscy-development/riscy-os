#ifndef __LIBC__STDARG_H__
#define __LIBC__STDARG_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
/* We use the GCC stdarg as it is compiler-specific. */
#    include_next <stdarg.h>
#else
#    error Only compatible with GCC right now.
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif
