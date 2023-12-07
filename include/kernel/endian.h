#ifndef __KERNEL_ENDIAN_H__
#define __KERNEL_ENDIAN_H__

#include <kernel/stdint.h>

/**
 * Is the system little endian?
 */
static inline bool
sys_little_endian(void) {
#ifdef CONFIG_LITTLE_ENDIAN
    return 1;
#elifdef CONFIG_BIG_ENDIAN
    return 0;
#elifdef CONFIG_VARIABLE_ENDIAN
    uint16_t x = 1;
    return *(uint8_t*)&x;
#else
#    error Improper configuration!
#endif
}

/**
 * Is the system big endian?
 */
static inline bool
sys_big_endian(void) {
    return !sys_little_endian();
}

/**
 * Byte-swap a 16 bit integer
 */
static inline uint16_t
bswap16(uint16_t x) {
#ifdef __GNUC__
    return __builtin_bswap16(x);
#else
    return (uint16_t)((x & 0xF) << 8) | (uint16_t)((x >> 8) & 0xF);
#endif
}

/**
 * Byte-swap a 32 bit integer
 */
static inline uint32_t
bswap32(uint32_t x) {
#ifdef __GNUC__
    return __builtin_bswap32(x);
#else
    return (((uint32_t)bswap16(x & 0xFF)) << 16)
           | ((uint32_t)bswap16((uint16_t)(x >> 16)) & 0xFF);
#endif
}

/**
 * Byte-swap a 64 bit integer
 */
static inline uint64_t
bswap64(uint64_t x) {
#ifdef __GNUC__
    return __builtin_bswap64(x);
#else
    return (((uint64_t)bswap32(x & 0xFFFF)) << 32)
           | ((uint64_t)bswap32((uint32_t)(x >> 32)) & 0xFFFF);
#endif
}

/**
 * 16 bit big endian to host endianness
 */
static inline uint16_t
be16toh(uint16_t x) {
    return sys_little_endian() ? bswap16(x) : x;
}

/**
 * 32 bit big endian to host endianness
 */
static inline uint32_t
be32toh(uint32_t x) {
    return sys_little_endian() ? bswap32(x) : x;
}

/**
 * 64 bit big endian to host endianness
 */
static inline uint64_t
be64toh(uint64_t x) {
    return sys_little_endian() ? bswap64(x) : x;
}

/**
 * 16 bit little endian to host.
 */
static inline uint16_t
le16toh(uint16_t x) {
    return sys_big_endian() ? bswap16(x) : x;
}

/**
 * 32 bit little endian to host.
 */
static inline uint32_t
le32toh(uint32_t x) {
    return sys_big_endian() ? bswap32(x) : x;
}

/**
 * 64 bit little endian to host.
 */
static inline uint64_t
le64toh(uint64_t x) {
    return sys_big_endian() ? bswap64(x) : x;
}

/*****************************************************************************/

#ifdef __cplusplus

/**
 * Byteswap an integer
 */
static inline auto
bswap(uint16_t x) {
    return bswap16(x);
}

/**
 * Byteswap an integer
 */
static inline auto
bswap(uint32_t x) {
    return bswap32(x);
}

/**
 * Byteswap an integer
 */
static inline auto
bswap(uint64_t x) {
    return bswap64(x);
}

/**
 * Big endian to host endian.
 */
static inline auto
betoh(uint16_t x) {
    return be16toh(x);
}

/**
 * Big endian to host endian.
 */
static inline auto
betoh(uint32_t x) {
    return be32toh(x);
}

/**
 * Big endian to host endian.
 */
static inline auto
betoh(uint64_t x) {
    return be64toh(x);
}

/**
 * Little endian to host endian.
 */
static inline auto
letoh(uint16_t x) {
    return le16toh(x);
}

/**
 * Little endian to host endian.
 */
static inline auto
letoh(uint32_t x) {
    return le32toh(x);
}

/**
 * Little endian to host endian.
 */
static inline auto
letoh(uint64_t x) {
    return le64toh(x);
}

#endif

#endif
