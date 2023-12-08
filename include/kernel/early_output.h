#ifndef __KERNEL_EARLY_OUTPUT_H__
#define __KERNEL_EARLY_OUTPUT_H__

#include<kernel/error.h>

#ifdef __cplusplus
extern "C" {
#endif

// This is the maximum number of putchar callbacks which can be registered
#define EARLY_OUTPUT_MAX_PUTCHAR 8

/*
 * "early_output" system for getting output extremely early during boot.
 *
 * All a driver needs to do is register a "putchar" callback.
 * So it can be done before any of the driver's true dependencies
 * (subsystems like kmalloc, device registration, paging, etc) 
 * are up and running.
 */

/*
 * Registers "putchar" as a callback function for early output
 *
 * Returns:
 *    KERR_SUCCESS on success
 *    KERR_FULL if the maximum number of callbacks have been registered
 *    KERR_EXIST if this callback has already been registered
 */
kerror_t register_early_putchar(void(*putchar)(char));

/*
 * Unregisters a "putchar" previously registered by "register_early_putchar"
 *
 * Returns:
 *     KERR_SUCCESS on success
 *     KERR_NO_EXIST if this callback hasn't been registered
 */
kerror_t unregister_early_putchar(void(*putchar)(char));

/*
 * Outputs a character on all functions registered using "register_early_putchar"
 */
void early_putchar(char c);

/*
 * Outputs a string using "early_putchar"
 */
void early_puts(const char *s);

#ifdef __cplusplus
}
#endif

#endif
