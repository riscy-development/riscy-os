/**
 * Defines the following language elements
 *
 * - NULL and nullptr_t
 * - size_t (and ssize_t)
 * - ptrdiff_t
 * - max_align_t
 * - wchar_t
 * - offsetof
 * - unreachable
 */

#ifndef __LIBC__STDDEF_H__
#define __LIBC__STDDEF_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Define NULL and nullptr_t
 *
 * NULL is just 0 (as always)
 *
 * nullptr_t is apparently typeof(nullptr), but typeof is an extension
 * GCC already defines nullptr_t for us
 * So support on other platforms is best effort (i.e. have you defined typeof?).
 */
#undef NULL
#define NULL 0

#if defined(__STDC_VERSION__) && !defined(__GNUC__)
/* GCC already defines nullptr_t */
typedef typeof(nullptr) nullptr_t;
#endif

/**
 * size_t and ssize_t
 *
 * Needs to be able to hold the result of sizeof
 *
 * We define it to the largest size we have, the word size.
 */
#if CONFIG_64_BIT
typedef uint64_t size_t;
typedef int64_t ssize_t;
#else
typedef uint32_t size_t;
typedef int32_t ssize_t;
#endif

/* NOLINTBEGIN */
_Static_assert(sizeof(size_t) == sizeof(sizeof(int)), "size_t not set correctly!");
_Static_assert(sizeof(ssize_t) == sizeof(sizeof(int)), "ssize_t not set correctly!");
/* NOLINTEND */

/**
 * ptrdiff_t
 *
 * Must be large enough to hold the difference of two pointers.
 *
 * A pointer is the size of a word, so its a signed word, or a ssize_t.
 */
typedef ssize_t ptrdiff_t;

/**
 * max_align_t
 *
 * A type whose alignment requirement is at least as strict (as large) as that
 * of every scalar type.
 *
 * We just make a struct of large types and then the compiler will figure it out.
 */
typedef struct {
    long long ll;
    long double ld;
    void* ptr;
} max_align_t;

/**
 * wchar_t
 *
 * Holds wide characters.
 *
 * Seems to default to int.
 */
typedef int wchar_t;

/**
 * offsetof
 *
 * Get the offset of a MEMBER in a struct of type TYPE.
 */
#ifdef __GNUC__
/* NOLINTNEXTLINE(readability-identifier-naming) */
#    define offsetof(__TYPE, __MEMBER) __builtin_offsetof(__TYPE, __MEMBER)
#else
/* https://en.wikipedia.org/wiki/Offsetof */
/* NOLINTNEXTLINE(readability-identifier-naming) */
#    define offsetof(__TYPE, __MEMBER) ((size_t) & (((__TYPE*)0)->__MEMBER))
#endif

/**
 * unreachable
 *
 * Tells the compiler this code cannot be reached.
 */
#if defined(__GNUC__) || defined(__clang__)
#    define unreachable() __builtin_unreachable() /* NOLINT */
#elif defined(_MSC_VER)
#    define unreachable() __assume(false)
#else
#    warning Unknown compiler, cannot define unreachable.
#endif

#ifdef __cplusplus
} // extern "C"
#endif

/* C++ nullptr type */
#ifdef __cplusplus
typedef decltype(nullptr) nullptr_t;
#endif

#endif
