#include "drivers/syscon.h"

#include "kernel/endian.h"
#include "kernel/of/fdt.h"

#include "print.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

static bool shutdown_found = false;
static volatile uint32_t* shutdown_register;
static uint32_t shutdown_mask;
static uint32_t shutdown_value;

kerror_t
syscon_shutdown_init(struct fdt* fdt)
{
    shutdown_found = false;

    // See if we have a generic "syscon-poweroff" node
    struct fdt_node* shutdown_node =
        fdt_find_compatible_node(fdt, NULL, "syscon-poweroff");
    if (shutdown_node == NULL) {
        return KERR_NO_EXIST;
    }

    // Get the phandle to the syscon this poweroff node is referring to
    struct fdt_prop* regmap = fdt_get_prop_by_name(fdt, shutdown_node, NULL, "regmap");
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
    struct fdt_prop* offset_prop =
        fdt_get_prop_by_name(fdt, shutdown_node, NULL, "offset");
    if (offset_prop == NULL) {
        return KERR_NO_EXIST;
    }

    size_t offset_cells = fdt_prop_num_cells(offset_prop);
    if (offset_cells != 1) {
        return KERR_NO_EXIST;
    }
    uint32_t offset = fdt_prop_get_cell(offset_prop, 0);

    // We care about the register at "offset" bytes into the syscon
    shutdown_register = (uint32_t*)(syscon_address + offset);

    // Try to get the 32-bit mask we need to use when writing
    bool found_mask = false;

    struct fdt_prop* mask_prop = fdt_get_prop_by_name(fdt, shutdown_node, NULL, "mask");
    if (mask_prop != NULL) {
        size_t mask_cells = fdt_prop_num_cells(mask_prop);
        if (mask_cells == 1) {
            found_mask = true;
            shutdown_mask = fdt_prop_get_cell(mask_prop, 0);
        }
    }

    if (!found_mask) {
        // All one's (effectively no mask)
        shutdown_mask = (uint32_t)(-1);
    }

    // Try to get the 32-bit value we actually write
    bool found_value = false;

    struct fdt_prop* value_prop =
        fdt_get_prop_by_name(fdt, shutdown_node, NULL, "value");
    if (value_prop != NULL) {
        size_t value_cells = fdt_prop_num_cells(value_prop);
        if (value_cells == 1) {
            found_value = true;
            shutdown_value = fdt_prop_get_cell(value_prop, 0);
        }
    }

    if (!found_mask && !found_value) {
        // We didn't find either (something is wrong)
        return KERR_NO_EXIST;
    }
    else if (!found_value) {
        // We found the mask but didn't find the value
        // (Use the mask as the value)
        shutdown_value = shutdown_mask;
    }

    // We found everything we needed to (enable the shutdown function)
    shutdown_found = true;
    return KERR_NONE;
}

static bool reboot_found = false;
static volatile uint32_t* reboot_register;
static uint32_t reboot_mask;
static uint32_t reboot_value;

// TODO: This function is a near exact clone of "syscon_shutdown_init"
//       We can probably make one new function which both
//       shutdown_init and reboot_init call with different parameters.

kerror_t
syscon_reboot_init(struct fdt* fdt)
{
    reboot_found = false;

    // See if we have a generic "syscon-poweroff" node
    struct fdt_node* reboot_node = fdt_find_compatible_node(fdt, NULL, "syscon-reboot");
    if (reboot_node == NULL) {
        return KERR_NO_EXIST;
    }

    // Get the phandle to the syscon this poweroff node is referring to
    struct fdt_prop* regmap = fdt_get_prop_by_name(fdt, reboot_node, NULL, "regmap");
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
    struct fdt_prop* offset_prop =
        fdt_get_prop_by_name(fdt, reboot_node, NULL, "offset");
    if (offset_prop == NULL) {
        return KERR_NO_EXIST;
    }

    size_t offset_cells = fdt_prop_num_cells(offset_prop);
    if (offset_cells != 1) {
        return KERR_NO_EXIST;
    }
    uint32_t offset = fdt_prop_get_cell(offset_prop, 0);

    // We care about the register at "offset" bytes into the syscon
    reboot_register = (uint32_t*)(syscon_address + offset);

    // Try to get the 32-bit mask we need to use when writing
    bool found_mask = false;

    struct fdt_prop* mask_prop = fdt_get_prop_by_name(fdt, reboot_node, NULL, "mask");
    if (mask_prop != NULL) {
        size_t mask_cells = fdt_prop_num_cells(mask_prop);
        if (mask_cells == 1) {
            found_mask = true;
            reboot_mask = fdt_prop_get_cell(mask_prop, 0);
        }
    }

    if (!found_mask) {
        // All one's (effectively no mask)
        reboot_mask = (uint32_t)(-1);
    }

    // Try to get the 32-bit value we actually write
    bool found_value = false;

    struct fdt_prop* value_prop = fdt_get_prop_by_name(fdt, reboot_node, NULL, "value");
    if (value_prop != NULL) {
        size_t value_cells = fdt_prop_num_cells(value_prop);
        if (value_cells == 1) {
            found_value = true;
            reboot_value = fdt_prop_get_cell(value_prop, 0);
        }
    }

    if (!found_mask && !found_value) {
        // We didn't find either (something is wrong)
        return KERR_NO_EXIST;
    }
    else if (!found_value) {
        // We found the mask but didn't find the value
        // (Use the mask as the value)
        reboot_value = reboot_mask;
    }

    // We found everything we needed to (enable the reboot function)
    reboot_found = true;
    return KERR_NONE;
}

[[noreturn]] syscon_shutdown()
{
    assert(shutdown_found);

    uint32_t curr_val = *shutdown_register;
    *shutdown_register = (curr_val & ~shutdown_mask) | (shutdown_value & shutdown_mask);

    unreachable();
}

[[noreturn]] void
syscon_reboot()
{
    assert(reboot_found);

    uint32_t curr_val = *reboot_register;
    *reboot_register = (curr_val & ~reboot_mask) | (reboot_value & reboot_mask);

    unreachable();
}
