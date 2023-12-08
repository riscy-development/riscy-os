#ifndef __KERNEL_STRING_H__
#define __KERNEL_STRING_H__

#include <kernel/stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#    define NULL 0
#endif

void memcpy(uint8_t* src, uint8_t* dest, size_t len);
int strlen(const char*);
int strcmp(const char* a, const char* b);

#ifdef __cplusplus
}
#endif

#endif