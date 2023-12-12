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
#define KERNEL_ALIGNED(b) __attribute__((aligned (b)))
#else 
#error "Alignment attribute is not supported!"
#endif

#endif
