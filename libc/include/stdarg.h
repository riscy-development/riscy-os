#ifndef __LIBC__STDARG_H__
#define __LIBC__STDARG_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__

typedef __builtin_va_list va_list;

#if defined __STDC_VERSION__ && __STDC_VERSION__ > 201710L
#define va_start(v, ...)	__builtin_va_start(v, 0)
#else
#define va_start(v,l)	__builtin_va_start(v,l)
#endif
#define va_end(v)	__builtin_va_end(v)
#define va_arg(v,l)	__builtin_va_arg(v,l)
#if !defined(__STRICT_ANSI__) || __STDC_VERSION__ + 0 >= 199900L \
    || __cplusplus + 0 >= 201103L
#define va_copy(d,s)	__builtin_va_copy(d,s)
#endif
#define __va_copy(d,s)	__builtin_va_copy(d,s)

#else
#    error Only compatible with GCC right now.
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif
