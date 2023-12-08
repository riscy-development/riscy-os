
#include <kernel/endian.h>
#include <kernel/of/fdt.h>

static bool
fdt_struct_ptr_invalid(struct fdt* fdt, void* ptr) {
    void* start = ((void*)fdt) + be32toh(fdt->off_dt_struct);
    void* end = start + be32toh(fdt->size_dt_struct);
    if (ptr < start || ptr >= end) {
        return true;
    }
    return false;
}

static uint32_t*
fdt_token_begin(struct fdt* fdt) {
    return (uint32_t*)(((void*)fdt) + be32toh(fdt->off_dt_struct));
}

static uint32_t*
fdt_next_token(struct fdt* fdt, uint32_t* token) {
    struct fdt_node* node;
    struct fdt_prop* prop;
    size_t str_len;
    size_t cell_len;

    uint32_t val = be32toh(*token);

    switch (val) {
        case FDT_NOP:
        case FDT_END_NODE:
            token++;
            break;
        case FDT_BEGIN_NODE:
            node = (struct fdt_node*)token;
            token++;
            str_len = (size_t)strlen(node->unit_name) + 1;
            // (Comment for Nino) Align to 4 bytes
            cell_len = (str_len >> 2) + ((str_len & 3) != 0);
            token += cell_len;
            break;
        case FDT_PROP:
            prop = (struct fdt_prop*)token;
            token++;
            str_len = fdt_prop_val_len(prop);
            // (Comment for Nino) Align to 4 bytes
            cell_len = (str_len >> 2) + ((str_len & 3) != 0);
            // (Comment for Nino) extra 2 is for "len" and "name_offset" fields
            token += (2ull + cell_len);
            break;
        case FDT_END:
        default:
            token = NULL;
            break;
    }

    if (fdt_struct_ptr_invalid(fdt, (void*)token)) {
        return NULL;
    }
    return token;
}

static uint32_t*
fdt_next_token_after_node(struct fdt* fdt, struct fdt_node* node) {
    uint32_t* token_ptr = (uint32_t*)node;
    if (node == NULL) {
        return NULL;
    }
    int depth = 0;

    do {
        uint32_t token = be32toh(*token_ptr);
        switch (token) {
            case FDT_BEGIN_NODE:
                depth++;
                break;
            case FDT_END_NODE:
                depth--;
                break;
            default:
                break;
        }

        token_ptr = fdt_next_token(fdt, token_ptr);

        // Premature ending
        if (token_ptr == NULL) {
            return NULL;
        }

    } while (depth > 0);

    return token_ptr;
}

enum fdt_error
fdt_verify(struct fdt* fdt) {
    if (be32toh(fdt->magic) != FDT_HEADER_MAGIC) {
        return FDT_BAD_MAGIC;
    }
    if (be32toh(fdt->last_comp_version) > FDT_COMPAT_VERSION) {
        return FDT_INCOMPATIBLE;
    }
    return FDT_VALID;
}

size_t
fdt_size(struct fdt* fdt) {
    return (size_t)be32toh(fdt->totalsize);
}

size_t
fdt_prop_val_len(struct fdt_prop* prop) {
    return (size_t)be32toh(prop->len);
}

void*
fdt_prop_val(struct fdt_prop* prop) {
    return (void*)(prop->val);
}

size_t
fdt_prop_name_offset(struct fdt_prop* prop) {
    return (size_t)be32toh(prop->name_offset);
}

struct fdt_node*
fdt_node_begin(struct fdt* fdt) {
    uint32_t* token_ptr = fdt_token_begin(fdt);
    while (token_ptr != NULL) {
        uint32_t token = be32toh(*token_ptr);
        if (token == FDT_BEGIN_NODE) {
            return (struct fdt_node*)token_ptr;
        }
        token_ptr = fdt_next_token(fdt, token_ptr);
    }
    return NULL;
}

struct fdt_node*
fdt_next_node(struct fdt* fdt, struct fdt_node* node) {
    uint32_t* token_ptr = fdt_next_token(fdt, (uint32_t*)node);
    while (token_ptr != NULL) {
        uint32_t token = be32toh(*token_ptr);
        if (token == FDT_BEGIN_NODE) {
            return (struct fdt_node*)token_ptr;
        }
        token_ptr = fdt_next_token(fdt, token_ptr);
    }
    return NULL;
}

struct fdt_prop*
fdt_node_prop_begin(struct fdt* fdt, struct fdt_node* node) {
    uint32_t* token_ptr = fdt_next_token(fdt, (uint32_t*)node);

    while (token_ptr != NULL) {
        uint32_t token = be32toh(*token_ptr);

        switch (token) {
            case FDT_BEGIN_NODE:
            case FDT_END_NODE:
            case FDT_END:
            default:
                return NULL;
            case FDT_PROP:
                return (struct fdt_prop*)token_ptr;
            case FDT_NOP:
                break;
        }
        token_ptr = fdt_next_token(fdt, token_ptr);
    }

    return NULL;
}

struct fdt_prop*
fdt_node_next_prop(struct fdt* fdt, struct fdt_prop* prop) {
    uint32_t* token_ptr = fdt_next_token(fdt, (uint32_t*)prop);

    while (token_ptr != NULL) {
        uint32_t token = be32toh(*token_ptr);

        switch (token) {
            // All Properties must come before subnodes in Ver. 17
            case FDT_BEGIN_NODE:
            case FDT_END_NODE:
            case FDT_END:
            default:
                return NULL;
            case FDT_PROP:
                return (struct fdt_prop*)token_ptr;
            case FDT_NOP:
                break;
        }
        token_ptr = fdt_next_token(fdt, token_ptr);
    }

    return NULL;
}

struct fdt_node*
fdt_node_subnode_begin(struct fdt* fdt, struct fdt_node* node) {
    uint32_t* token_ptr = fdt_next_token(fdt, (uint32_t*)node);

    while (token_ptr != NULL) {
        uint32_t token = be32toh(*token_ptr);

        switch (token) {
            case FDT_END_NODE:
            case FDT_END:
            default:
                return NULL;
            case FDT_BEGIN_NODE:
                return (struct fdt_node*)token_ptr;
            case FDT_NOP:
            case FDT_PROP: // Properties come before subnodes
                break;
        }
        token_ptr = fdt_next_token(fdt, token_ptr);
    }

    return NULL;
}

struct fdt_node*
fdt_node_next_subnode(struct fdt* fdt, struct fdt_node* subnode) {
    uint32_t* token_ptr = fdt_next_token_after_node(fdt, subnode);

    while (token_ptr != NULL) {
        uint32_t token = be32toh(*token_ptr);

        switch (token) {
            case FDT_END_NODE:
            case FDT_END:
            default:
                return NULL;
            case FDT_BEGIN_NODE:
                return (struct fdt_node*)token_ptr;
            case FDT_NOP:
            case FDT_PROP: // Properties come before subnodes
                           // (so this shouldn't happen)
                           // But we'll be safe anyways
                break;
        }
        token_ptr = fdt_next_token(fdt, token_ptr);
    }

    return NULL;
}

const char*
fdt_string_from_offset(struct fdt* fdt, size_t offset) {
    const char* strings_start = (char*)fdt + be32toh(fdt->off_dt_strings);
    const char* strings_end = strings_start + be32toh(fdt->size_dt_strings);

    const char* s = strings_start + offset;
    if (s < strings_start || s >= strings_end) {
        return NULL;
    }

    return s;
}

/*
 * Helper Functions
 */

const char*
fdt_node_name(struct fdt_node* node) {
    return node->unit_name;
}

const char*
fdt_prop_name(struct fdt* fdt, struct fdt_prop* prop) {
    return fdt_string_from_offset(fdt, fdt_prop_name_offset(prop));
}

struct fdt_prop*
fdt_get_prop_by_name(
    struct fdt* fdt, struct fdt_node* node, struct fdt_prop* start, const char* name
) {
    struct fdt_prop* prop;
    if (start != NULL) {
        prop = fdt_node_next_prop(fdt, start);
    } else {
        prop = fdt_node_prop_begin(fdt, node);
    }

    while (prop != NULL) {
        const char* prop_name = fdt_prop_name(fdt, prop);
        if (prop_name != NULL && strcmp(name, prop_name) == 0) {
            return prop;
        }
        prop = fdt_node_next_prop(fdt, prop);
    }
    return NULL;
}

bool
fdt_node_is_compatible(struct fdt* fdt, struct fdt_node* node, const char* compat) {
    struct fdt_prop* prop = fdt_get_prop_by_name(fdt, node, NULL, "compatible");

    if (prop == NULL) {
        return 0;
    }

    size_t strings_len = fdt_prop_val_len(prop);
    const char* strings = fdt_prop_val(prop);

    while (strings_len > 0) {
        size_t curr_len = (size_t)strlen(strings) + 1;
        const char* curr_string = strings;

        if (strcmp(curr_string, compat) == 0) {
            return true;
        }

        strings += curr_len;
        strings_len -= curr_len;
    }

    return false;
}

struct fdt_node*
fdt_find_compatible_node(struct fdt* fdt, struct fdt_node* start, const char* compat) {
    struct fdt_node* node;
    if (start != NULL) {
        node = fdt_next_node(fdt, start);
    } else {
        node = fdt_node_begin(fdt);
    }

    while (node != NULL) {
        if (fdt_node_is_compatible(fdt, node, compat)) {
            return node;
        }
        node = fdt_next_node(fdt, node);
    }

    return NULL;
}
