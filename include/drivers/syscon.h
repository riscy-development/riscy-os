#ifndef __KERNEL__SYSCON_H__
#define __KERNEL__SYSCON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "kernel/error.h"
#include "kernel/of/fdt.h"

#include <stdint.h>

/**
 * Init the syscon system.
 */
[[nodiscard]] kerror_t syscon_init(struct fdt* fdt);

/**
 * Get the syscon register for our board.
 *
 * @returns The memory address of the syscon register.
 */
volatile uint32_t* syscon_mmio_register();

/**
 * Shutdown the system.
 */
[[noreturn]] void syscon_shutdown();

/**
 * Reboot the system.
 */
[[noreturn]] void syscon_reboot();

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
