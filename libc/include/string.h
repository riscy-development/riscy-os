#ifndef __LIBC__STRING_H__
#define __LIBC__STRING_H__

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

/**
 * Get the length of the given null-terminated string.
 *
 * This DOES NOT include the null terminator.
 *
 * The behavior is undefined if `str` is not a pointer
 * to a null-terminated byte string.
 *
 * @param str A null-terminated byte-string.
 *
 * @returns The length of `str`.
 */
size_t strlen(const char* str);

/**
 * Compare two null-terminated byte strings lexicographically.
 *
 * Characters interpreted as `unsigned char`.
 *
 * Undefined behavior if either string is not null-terminated.
 *
 * @param lhs The first string to compare.
 * @param rhs The second string to compare.
 *
 * @returns -1 if lhs comes before rhs, 1 if rhs comes before lhs, and 0 if equal.
 */
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
 * Compare the first `count` bytes of `lhs` and `rhs` lexicographically.
 *
 * Both arrays are interpreted as having elements of `uint8_t`.
 *
 * Behavior undefined if access occurs beyond the end of either array.
 * Behavior undefined if either array is a NULL pointer.
 *
 * @param lhs The first array to compare.
 * @param rhs The second array to compare.
 * @param count How many bytes to compare.
 *
 * @returns -1 if lhs comes first, 1 if rhs comes first, and 0 if they are equal.
 */
int memcmp(const void* lhs, const void* rhs, size_t count);

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
 * @returns The destination array (`dest`).
 */
void* memcpy(void* restrict dest, const void* restrict src, size_t count);

/**
 * Copies count characters from `src` to `dest`
 *
 * Both arrays are interpreted as `uint8_t*`.
 *
 * The arrays may overlap.
 *
 * Behavior is undefined if access occurs beyond the end of `dest`.
 * Behavior is undefined if either array is NULL.
 *
 * @param src Source array.
 * @param dest Destination array.
 * @param count How many bytes to copy.
 *
 * @returns The destination array (`dest`).
 */
void* memmove(void* dest, const void* src, size_t count);

/**
 * Copies (uint8_t)ch into each of the first `count` chars in `dest`.
 *
 * Behavior undefined if access occurs beyond the end of the `dest array.
 * Behavior undefined if `dest` is a NULL pointer.
 *
 * @param dest The destination array.
 * @param ch The value to set in `dest`.
 * @param count How many bytes to set in `dest`.
 *
 * @returns The destination array (`dest`).
 */
void* memset(void* dest, int ch, size_t count);

/**
 * Copies (uint8_t)ch into each of the first `count` chars in `dest`.
 *
 * Same as `memset`, but the compiler isn't allowed to optimize it out.
 *
 * Behavior undefined if access occurs beyond the end of the `dest` array.
 * Behavior undefined if `dest` is a NULL pointer.
 *
 * @param dest The destination array.
 * @param ch The value to set in `dest`.
 * @param count How many bytes to set in `dest`.
 *
 * @returns The destination array (`dest`).
 */
void* memset_explicit(void* dest, int ch, size_t count);

#if 0 /* NOLINT: TODO */
void* memchr(const void* ptr, int ch, size_t count);
void* memccpy(void* restrict dest, const void* restrict src, int c, size_t count);
#endif

/*****************************************************************************
 *                              MISCELLANEOUS                                *
 *****************************************************************************/
#if 0 /* NOLINT: TODO */
char* strerror(int errnum); /* WHY ISN'T THIS CONST??? */
#endif

#ifdef __cplusplus
}
#endif

#endif
