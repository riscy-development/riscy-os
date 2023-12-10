#ifndef __KERNEL__SYSCON_H__
#define __KERNEL__SYSCON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "kernel/error.h"
#include "kernel/of/fdt.h"

#include <stdint.h>

/**
 * Try to find and initialize a generic "syscon-poweroff" node in the Device Tree.
 */
[[nodiscard]] kerror_t syscon_shutdown_init(struct fdt* fdt);

/**
 * Try to find and initialize a generic "syscon-reboot" node in the Device Tree.
 */
[[nodiscard]] kerror_t syscon_reboot_init(struct fdt* fdt);

/**
 * Shutdown the system.
 *
 * Calling this requires "syscon_shutdown_init" has been called and returned KERR_SUCCESS
 */
[[noreturn]] void syscon_shutdown();

/**
 * Reboot the system.
 *
 * Calling this requires "syscon_reboot_init" has been called and returned KERR_SUCCESS
 */
[[noreturn]] void syscon_reboot();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
