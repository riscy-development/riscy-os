#ifndef __LIBC__STDLIB_H__
#define __LIBC__STDLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#warning The stdlib header is not complete.

/**
 * Causes abnormal program termination.
 *
 * Functions passed to `atexit()` are not called.
 *
 * No guarantees are made on cleaning up system resources.
 *
 * This function does not return.
 */
[[noreturn]] void abort();

#if 0 /* NOLINT: TODO */
[[noreturn]] void exit(int exit_code);
[[noreturn]] void quick_exit(int exit_code);
[[noreturn]] void _Exit(int exit_code);

int atexit(void (*func)(void));
int at_quick_exit(void (*func)(void));

#  define EXIT_SUCCESS /*implementation defined*/
#  define EXIT_FAILURE /*implementation defined*/
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
