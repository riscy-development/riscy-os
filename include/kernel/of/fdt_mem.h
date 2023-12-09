#ifndef __KERNEL_FDT_MEM_H__
#define __KERNEL_FDT_MEM_H__

#include <kernel/error.h>
#include <kernel/mem/boot.h>
#include <kernel/of/fdt.h>

kerror_t fdt_boot_mem_init(struct fdt* fdt);

#endif
