#ifndef __KERNEL_STRING_H__
#define __KERNEL_STRING_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#warning The string library is not fully complete yet.

/*****************************************************************************
 *                           STRING MANIPULATION                             *
 *****************************************************************************/

#if 0 /* NOLINT: TODO */
char* strcpy(char* restrict dest, const char* restrict src);
char* strncpy(char* restrict dest, const char* restrict src, size_t count);
char* strcat(char* restrict dest, const char* restrict src);
char* strncat(char* restrict dest, const char* restrict src, size_t count);
size_t strxfrm(char* restrict dest, const char* restrict src, size_t count);
char* strdup(const char* src);
char* strndup(const char* src, size_t size);
#endif

/*****************************************************************************
 *                           STRING EXAMINATION                              *
 *****************************************************************************/
size_t strlen(const char* str);
int strcmp(const char* lhs, const char* rhs);

#if 0 /* NOLINT: TODO */
int strncmp(const char* lhs, const char* rhs, size_t count);
int strcoll(const char* lhs, const char* rhs);
char* strchr(const char* str, int ch);
char* strrchr(const char* str, int ch);
size_t strspn(const char* dest, const char* src);
size_t strcspn(const char* dest, const char* src);
char* strpbrk(const char* dest, const char* breakset);
char* strstr(const char* str, const char* substr);
char* strtok(char* restrict str, const char* restrict delim);
#endif

/*****************************************************************************
 *                       CHARACTER ARRAY MANIPULATION                        *
 *****************************************************************************/

/**
 * Copy `count` bytes from `src` to `dest`.
 *
 * Behavior is undefined when:
 *   - `dest` and `src` overlap
 *   - Either of `dest` or `src` is NULL
 *   - `count` exceeds the size of `dest` or `src`
 *
 * @param dest The destination memory location.
 * @param src The source memory location.
 * @param count How many bytes to copy.
 *
 * @returns The destination (`dest`).
 */
void* memcpy(void* restrict dest, const void* restrict src, size_t count);

void* memset(void* dest, int ch, size_t count);
void* memset_explicit(void* dest, int ch, size_t count); /* just call memset here */
void* memmove(void* dest, const void* src, size_t count);

#if 0 /* NOLINT: TODO */
void* memchr(const void* ptr, int ch, size_t count);
int memcmp(const void* lhs, const void* rhs, size_t count);
void* memccpy(void* restrict dest, const void* restrict src, int c, size_t count);
#endif

/*****************************************************************************
 *                              MISCELLANEOUS                                *
 *****************************************************************************/
char* strerror(int errnum); /* WHY ISN'T THIS CONST??? */

#ifdef __cplusplus
}
#endif

#endif
