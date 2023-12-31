#ifndef __KERNEL_OF_FDT_H__
#define __KERNEL_OF_FDT_H__

#include "kernel/attribute.h"

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// Device Tree Version 17 Specification
// https://devicetree-specification.readthedocs.io/en/stable/flattened-format.html#figure-device-tree-structure

// TODO: Bad debugging system (should integrate into Kconfig or delete entirely later)
#define FDT_DEBUG_DEBUG_NAMELESS_NODES
//

#define FDT_COMPAT_VERSION 17

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
} KERNEL_PACKED KERNEL_ALIGNED(sizeof(void*));

struct fdt_node {
    uint32_t token;
    char unit_name[];
} KERNEL_PACKED KERNEL_ALIGNED(4);

struct fdt_prop {
    uint32_t token;
    uint32_t len;
    uint32_t name_offset;
    uint8_t val[];
} KERNEL_PACKED KERNEL_ALIGNED(4);

struct fdt_reserve_entry {
    uint64_t address;
    uint64_t size;
} KERNEL_PACKED KERNEL_ALIGNED(sizeof(void*));

#define FDT_HEADER_MAGIC 0xd00dfeed

#define FDT_BEGIN_NODE   0x1
#define FDT_END_NODE     0x2
#define FDT_PROP         0x3
#define FDT_NOP          0x4
#define FDT_END          0x9

/* --------------------
 *  CORE FUNCTIONALITY
 * -------------------- */

enum fdt_error {
    FDT_VALID,

    FDT_BAD_MAGIC,
    FDT_INCOMPATIBLE,
};

/*
 * Make sure that this FDT is both Valid, and a version which we are equipped to parse
 */
enum fdt_error fdt_verify(struct fdt* fdt);

/*
 * Get the size of this FDT in bytes
 */
size_t fdt_size(struct fdt* fdt);

/*
 * Get the maximum depth of this FDT
 * where a "root" node is at depth 0 and subnodes increase the depth by 1
 */
word_t fdt_max_depth(struct fdt* fdt);

/*
 * Get the first FDT reserve_entry (or NULL if there are none)
 */
struct fdt_reserve_entry* fdt_reserve_entry_begin(struct fdt* fdt);

/*
 * Returns the next reserve_entry in the FDT (or NULL)
 */
struct fdt_reserve_entry*
fdt_next_reserve_entry(struct fdt* fdt, struct fdt_reserve_entry* entry);

/*
 * Get the base address of this reserve entry
 */
void* fdt_reserve_entry_address(struct fdt_reserve_entry* entry);

/*
 * Get the size in bytes of this reserve entry
 */
size_t fdt_reserve_entry_size(struct fdt_reserve_entry* entry);

/*
 * Get the length of the value of this property in bytes
 */
size_t fdt_prop_val_len(struct fdt_prop* prop);

/*
 * Get the raw data of this property
 */
void* fdt_prop_val(struct fdt_prop* prop);

/*
 * Get the offset into the string block with the name of this property
 */
size_t fdt_prop_name_offset(struct fdt_prop* prop);

/*
 * Get the first node in the entire FDT
 */
struct fdt_node* fdt_node_begin(struct fdt* fdt);

/*
 * Returns the next node in the tree (or NULL) traversing the entire tree regardless of
 * depth
 */
struct fdt_node* fdt_next_node(struct fdt* fdt, struct fdt_node* node, word_t* depth);

/*
 * Returns the first property in this node (or NULL if there are none)
 */
struct fdt_prop* fdt_node_prop_begin(struct fdt* fdt, struct fdt_node* node);

/*
 * Returns the next property after this one in the same node (or NULL)
 */
struct fdt_prop* fdt_node_next_prop(struct fdt* fdt, struct fdt_prop* prop);

/*
 * Returns the first subnode of this node (or NULL if there are none)
 */
struct fdt_node* fdt_node_subnode_begin(struct fdt* fdt, struct fdt_node* node);

/*
 * Returns the next subnode after this one (or NULL)
 */
struct fdt_node* fdt_node_next_subnode(struct fdt* fdt, struct fdt_node* subnode);

/*
 * Populates the provided array parents, of size parents_size,
 * with the parent node(s) of "node".
 *
 * Returns the number of ancestors added to parents[]
 *
 * "parents" will go from closest ancestor to furthest as the index increases
 */
size_t fdt_node_get_parents(
    struct fdt* fdt,
    struct fdt_node* node,
    struct fdt_node* parents[],
    size_t parents_size
);

/*
 * Get the string at offset "offset" in the string block
 */
const char* fdt_string_from_offset(struct fdt* fdt, size_t offset);

/*
 * Get the string representing the unit name of this node
 */
const char* fdt_node_name(struct fdt_node* node);

/* ------------------
 *  HELPER FUNCTIONS
 * ------------------ */

/*
 * Get the string representing the name of this property
 */
const char* fdt_prop_name(struct fdt* fdt, struct fdt_prop* prop);

/*
 * Find a property in "node" named "name" (or NULL if none exist)
 * The search will begin on property "start" (exclusive)
 * or if "start" is NULL, the first property in the node (inclusive)
 */
struct fdt_prop* fdt_get_prop_by_name(
    struct fdt* fdt, struct fdt_node* node, struct fdt_prop* start, const char* name
);

/*
 * Find a node in the device tree compatible with "compat"
 * The search will begin on node "start" (exclusive)
 * or if "start" is NULL, the first node in the FDT (inclusive)
 */
struct fdt_node*
fdt_find_compatible_node(struct fdt* fdt, struct fdt_node* start, const char* compat);

/*
 * Find a node in the device tree with "device_type" equal to "type"
 * The search will begin on node "start" (exclusive)
 * or if "start" is NULL, the first node in the FDT (inclusive)
 */
struct fdt_node*
fdt_find_node_by_device_type(struct fdt* fdt, struct fdt_node* start, const char* type);

/*
 * Find a node in the device tree with unit_name equal to "name"
 * The search will begin on node "start" (exclusive)
 * or if "start" is NULL, the first node in the FDT (inclusive)
 *
 * TODO: THIS IS NOT CORRECT YET
 *       really to be useful, we should only compare "name" to the part of the string
 * before "@", which we currently don't do.
 */
struct fdt_node*
fdt_find_node_by_unit_name(struct fdt* fdt, struct fdt_node* start, const char* name);

/*
 * Find the node referenced by "phandle"
 * The search will begin on node "start" (exclusive)
 * or if "start" is NULL, the first node in the FDT (inclusive)
 */
struct fdt_node*
fdt_find_node_by_phandle(struct fdt* fdt, struct fdt_node* start, uint32_t phandle);

/*
 * Returns true if "node" is compatible with "compat", else false
 */
bool fdt_node_is_compatible(struct fdt* fdt, struct fdt_node* node, const char* compat);

/*
 * Returns the closest definition of property "prop_name",
 * first checking the node itself, and then going to it's parent, grandparent, etc.
 *
 * Returns NULL if none are found.
 */
struct fdt_prop* fdt_node_get_inherited_prop(
    struct fdt* fdt, struct fdt_node* node, const char* prop_name
);

/*
 * Returns the #address-cells inherited property for this node
 */
uint32_t fdt_node_get_address_cells(struct fdt* fdt, struct fdt_node* node);

/*
 * Returns the #size-cells inherited property for this node
 */
uint32_t fdt_node_get_address_cells(struct fdt* fdt, struct fdt_node* node);

/*
 * Assumes this property contains an array of cells (u32's) and returns how many cells
 * long it is
 */
size_t fdt_prop_num_cells(struct fdt_prop* prop);

/*
 * Assumes this property contains an array of cells (u32's) and returns the cell at
 *index
 *.
 */
uint32_t fdt_prop_get_cell(struct fdt_prop* prop, size_t index);

/*
 * Returns the number of register blocks in the "reg" property of this node (0 if there
 * is no reg property)
 */
size_t fdt_node_num_register_blocks(struct fdt* fdt, struct fdt_node* node);

/*
 * Reads the "reg" property using "#address-cells" and "#size-cells",
 * Set's "address" and "size" accordingly
 *
 * If an error occurs, "size" is set to zero.
 */
void fdt_node_get_register_block(
    struct fdt* fdt, struct fdt_node* node, size_t index, void** address, size_t* size
);

#ifdef __cplusplus
}
#endif

#endif
