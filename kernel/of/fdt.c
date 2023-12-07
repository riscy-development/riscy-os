#include <kernel/endian.h>
#include <kernel/of/fdt.h>

int
verify_fdt(struct fdt* fdt) {
    return (be32toh(fdt->last_comp_version) < FDT_COMPAT_VERSION);
}

size_t
fdt_size(struct fdt* fdt) {
    return (size_t)be32toh(fdt->totalsize);
}
