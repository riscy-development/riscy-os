#ifndef __KERNEL_ENDIAN_H__
#define __KERNEL_ENDIAN_H__

#include<kernel/stdint.h>

static inline int sys_little_endian(void) {
#ifdef CONFIG_LITTLE_ENDIAN
	return 1;
#endif
#ifdef CONFIG_BIG_ENDIAN
	return 0;
#endif
#ifdef CONFIG_VARIABLE_ENDIAN
	uint16_t x = 1;
	return *(uint8_t*)&x;
#endif
}

static inline int sys_big_endian(void) {
	return !sys_little_endian();
}

static inline uint16_t bswap16(uint16_t x) { return (uint16_t)((x & 0xFF)<<8) | (uint16_t)((x>>8) & 0xFF); }
static inline uint32_t bswap32(uint32_t x) { return (((uint32_t)bswap16(x & 0xFFFF))<<16) | ((uint32_t)bswap16((uint16_t)(x>>16)) & 0xFFFF); }
static inline uint64_t bswap64(uint64_t x) { return (((uint64_t)bswap32(x & 0xFFFFFFFF))<<32) | ((uint64_t)bswap32((uint32_t)(x>>32)) & 0xFFFFFFFF); }

static inline uint16_t be16toh(uint16_t x) { return sys_little_endian() ? bswap16(x) : x; }
static inline uint32_t be32toh(uint32_t x) { return sys_little_endian() ? bswap32(x) : x; }
static inline uint64_t be64toh(uint64_t x) { return sys_little_endian() ? bswap64(x) : x; }

static inline uint16_t le16toh(uint16_t x) { return sys_big_endian() ? bswap16(x) : x; }
static inline uint32_t le32toh(uint32_t x) { return sys_big_endian() ? bswap32(x) : x; }
static inline uint64_t le64toh(uint64_t x) { return sys_big_endian() ? bswap64(x) : x; }

#endif
