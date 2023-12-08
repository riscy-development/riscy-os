#ifndef __KERNEL_OF_FDT_H__
#define __KERNEL_OF_FDT_H__

#include <stdint.h>
#include <string.h>

#define FDT_COMPAT_VERSION 17

#ifdef __cplusplus
extern "C" {
#endif

struct fdt {
    uint32_t magic;
    uint32_t totalsize;
    uint32_t off_dt_struct;
    uint32_t off_dt_strings;
    uint32_t off_mem_rsvmap;
    uint32_t version;
    uint32_t last_comp_version;
    uint32_t boot_cpuid_phys;
    uint32_t size_dt_strings;
    uint32_t size_dt_struct;
};

struct fdt_node {
    uint32_t magic;
    char unit_name[];
};

struct fdt_property {
    uint32_t len;
    uint32_t name_offset;
};

#define FDT_BEGIN_NODE 0x0
#define FDT_END_NODE   0x2
#define FDT_PROP       0x3
#define FDT_NOP        0x4
#define FDT_END        0x9

int verify_fdt(struct fdt* fdt);
size_t fdt_size(struct fdt* fdt);

#ifdef __cplusplus
}
#endif

#endif
