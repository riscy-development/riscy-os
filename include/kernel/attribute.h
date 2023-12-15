#ifndef __KERNEL__ATTRIBUTE_H__
#define __KERNEL__ATTRIBUTE_H__

#ifdef __GNUC__
#  define KERNEL_PACKED __attribute__((packed))
#else
#  error "Packed structs are not supported!"
#endif

#ifdef __GNUC__
#  define KERNEL_SECTION(sec_str) __attribute__((section(sec_str)))
#else
#  error "Section attributes are not supported!"
#endif

#ifdef __GNUC__
#  define KERNEL_ALIGNED(b) __attribute__((aligned(b)))
#else
#  error "Alignment attribute is not supported!"
#endif

/*
 * "sym" is an extern symbol (usually defined in a linker script) of unknown type.
 *
 * (By default it is an int[], which because word_t 
 *  should be used in place of "int" should cause 
 *  a compile time error when used without casting)
 */
#define EXTERN_SYMBOL(sym) extern int sym[]

/*
 * "sym" is an extern symbol with type "pointed_to_type[]"
 * so that it can be used as type "pointed_to_type*" without cast.
 */
#define EXTERN_PTR_SYMBOL(sym, pointed_to_type) extern pointed_to_type sym[]

#endif
