#ifndef __LIBC__STDLIB_H__
#define __LIBC__STDLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
