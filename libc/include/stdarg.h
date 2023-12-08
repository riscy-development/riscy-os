#ifndef __LIBC__STDARG_H__
#define __LIBC__STDARG_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__

typedef __builtin_va_list va_list;
#define va_start __builtin_va_start
#define va_end __builtin_va_end
#define va_arg __builtin_va_arg

#else
#    error Only compatible with GCC right now.
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif
