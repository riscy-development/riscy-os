#ifndef __KERNEL__ERROR_H__
#define __KERNEL__ERROR_H__

/*
 * Defines Common Error Codes which can be used by many different internal kernel API's
 *
 * Each API needs to define exactly what each error code means but these enum(s) should
 * fit the vast majority of use cases
 */

#ifdef __cplusplus
extern "C" {
#endif

enum kerror {
    KERR_NONE = 0, // No errors occurred
    KERR_SUCCESS = KERR_NONE,

    KERR_EXIST,    // Trying to create something which already exists
    KERR_NO_EXIST, // Trying to use something which doesn't exist

    KERR_FULL,  // Trying to insert/push into a full data structure
    KERR_EMPTY, // Trying to read/pop from an empty data structure

    KERR_INVALID, // Some data is malformed

    KERR_BUSY, // Less so an error, but an opportunity to do other work and try again
               // later

    KERR_UNIMPL, // This feature is not implemented
                 // (call did nothing and should have zero side effects)

    KERR_UNKNOWN, // Unknown error (catch all DO NOT USE WITHOUT GOOD REASON)
};
typedef enum kerror kerror_t;

#ifdef __cplusplus
}
#endif

#endif
