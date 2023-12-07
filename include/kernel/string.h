#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define NULL 0
typedef typeof(sizeof(int)) size_t;

int memcpy(uint8_t *src, uint8_t *dest, size_t len);
int strlen(char *);
int strcmp(char *a, char *b);

#ifdef __cplusplus
}
#endif
