#ifndef __LIBC_STDIO_H__
#define __LIBC_STDIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#warning stdio.h is incomplete, please be wary when using.

int printf(const char *fmt, ...);

#ifdef KERNEL
int printk(const char *fmt, ...);
#endif

#ifdef __cplusplus
}
#endif

#endif
