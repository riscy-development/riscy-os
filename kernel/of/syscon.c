#include "kernel/of/syscon.h"

#include "kernel/endian.h"
#include "kernel/of/fdt.h"
#include "print.h"

#include <assert.h>
#include <stddef.h>

static volatile uint32_t* SYSCON_ADDR = 0; /* NOLINT */

#define SYSCON_SHUTDOWN 0x5555
#define SYSCON_REBOOT   0x7777

void
syscon_init(struct fdt* fdt)
{
    /* Get our syscon FDT node */
    struct fdt_node* syscon = fdt_find_compatible_node(fdt, NULL, "syscon");
    assert(syscon);

    puts("Found syscon at ");
    puts(fdt_node_name(syscon));
    putchar('\n');

    /* Get the reg property */
    struct fdt_prop* syscon_reg = fdt_get_prop_by_name(fdt, syscon, NULL, "reg");

    /* Should be 4 x 32 bit values */
    uint32_t len = be32toh(syscon_reg->len);
    assert(len == 4 * sizeof(uint32_t));

    /* The second one is the syscon address */
    uint32_t* values = (uint32_t*)(syscon_reg->val); /* NOLINT */

    uint32_t raw_syscon_addr = be32toh(values[1]);
    assert(raw_syscon_addr != NULL);

    /* Set the syscon address */
    assert(SYSCON_ADDR == NULL); /* should only ever be called once */
    SYSCON_ADDR = (volatile uint32_t*)raw_syscon_addr; /* NOLINT */
}

volatile uint32_t*
syscon_mmio_register()
{
    assert(SYSCON_ADDR);
    return SYSCON_ADDR;
}

[[noreturn]] void
syscon_shutdown()
{
    assert(SYSCON_ADDR);

    *SYSCON_ADDR = SYSCON_SHUTDOWN;
    unreachable();
}

[[noreturn]] void
syscon_reboot()
{
    assert(SYSCON_ADDR);

    *SYSCON_ADDR = SYSCON_REBOOT;
    unreachable();
}
