#include "drivers/syscon.h"

#include "kernel/endian.h"
#include "kernel/of/fdt.h"

#include "print.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

struct generic_syscon_action {
    bool found;
    volatile uint32_t* reg;
    uint32_t mask;
    uint32_t value;
};

static struct generic_syscon_action shutdown_action = {
    .found = false,
    .reg = NULL,
    .mask = NULL,
    .value = NULL,
};

static struct generic_syscon_action reboot_action = {
    .found = false,
    .reg = NULL,
    .mask = NULL,
    .value = NULL,
};

static kerror_t
syscon_init_generic_action(
    struct generic_syscon_action* action, void* fdt, const char* compat
)
{
    action->found = false;

    // See if we have a generic "syscon-poweroff" node
    struct fdt_node* node = fdt_find_compatible_node(fdt, NULL, compat);
    if (node == NULL) {
        return KERR_NO_EXIST;
    }

    // Get the phandle to the syscon this poweroff node is referring to
    struct fdt_prop* regmap = fdt_get_prop_by_name(fdt, node, NULL, "regmap");
    if (regmap == NULL) {
        return KERR_NO_EXIST;
    }

    size_t regmap_cells = fdt_prop_num_cells(regmap);
    if (regmap_cells != 1) {
        return KERR_NO_EXIST;
    }
    uint32_t syscon_phandle = fdt_prop_get_cell(regmap, 0);

    // Use the phandle to get the syscon node
    struct fdt_node* syscon = fdt_find_node_by_phandle(fdt, NULL, syscon_phandle);
    if (syscon == NULL) {
        return KERR_NO_EXIST;
    }

    // Make sure the syscon has at least 1 register block
    size_t syscon_reg_block_count = fdt_node_num_register_blocks(fdt, syscon);
    if (syscon_reg_block_count < 1) {
        return KERR_NO_EXIST;
    }

    // Get the address and size of the syscon register block
    void* syscon_address;
    size_t syscon_size;
    fdt_node_get_register_block(fdt, syscon, 0, &syscon_address, &syscon_size);

    if (syscon_size == 0) {
        // Error reading register block (or it's actually just zero sized)
        // (Both would be bad)
        return KERR_NO_EXIST;
    }

    // Get the offset into the register block of our 32-bit register
    struct fdt_prop* offset_prop = fdt_get_prop_by_name(fdt, node, NULL, "offset");
    if (offset_prop == NULL) {
        return KERR_NO_EXIST;
    }

    size_t offset_cells = fdt_prop_num_cells(offset_prop);
    if (offset_cells != 1) {
        return KERR_NO_EXIST;
    }
    uint32_t offset = fdt_prop_get_cell(offset_prop, 0);

    // We care about the register at "offset" bytes into the syscon
    action->reg = (uint32_t*)(syscon_address + offset);

    // Try to get the 32-bit mask we need to use when writing
    bool found_mask = false;

    struct fdt_prop* mask_prop = fdt_get_prop_by_name(fdt, node, NULL, "mask");
    if (mask_prop != NULL) {
        size_t mask_cells = fdt_prop_num_cells(mask_prop);
        if (mask_cells == 1) {
            found_mask = true;
            action->mask = fdt_prop_get_cell(mask_prop, 0);
        }
    }

    if (!found_mask) {
        // All one's (effectively no mask)
        action->mask = (uint32_t)(-1);
    }

    // Try to get the 32-bit value we actually write
    bool found_value = false;

    struct fdt_prop* value_prop = fdt_get_prop_by_name(fdt, node, NULL, "value");
    if (value_prop != NULL) {
        size_t value_cells = fdt_prop_num_cells(value_prop);
        if (value_cells == 1) {
            found_value = true;
            action->value = fdt_prop_get_cell(value_prop, 0);
        }
    }

    if (!found_mask && !found_value) {
        // We didn't find either (something is wrong)
        return KERR_NO_EXIST;
    }
    else if (!found_value) {
        // We found the mask but didn't find the value
        // (Use the mask as the value)
        action->value = action->mask;
    }

    // We found everything we needed to (enable the shutdown function)
    action->found = true;
    return KERR_NONE;
}

kerror_t
syscon_shutdown_init(struct fdt* fdt)
{
    return syscon_init_generic_action(&shutdown_action, fdt, "syscon-poweroff");
}

kerror_t
syscon_reboot_init(struct fdt* fdt)
{
    return syscon_init_generic_action(&reboot_action, fdt, "syscon-reboot");
}

[[noreturn]] syscon_shutdown(void)
{
    assert(shutdown_action.found);

    uint32_t curr_val = *shutdown_action.reg;
    *shutdown_action.reg = (curr_val & ~shutdown_action.mask)
                           | (shutdown_action.value & shutdown_action.mask);

    unreachable();
}

[[noreturn]] void
syscon_reboot(void)
{
    assert(reboot_action.found);

    uint32_t curr_val = *reboot_action.reg;
    *reboot_action.reg =
        (curr_val & ~reboot_action.mask) | (reboot_action.value & reboot_action.mask);

    unreachable();
}
