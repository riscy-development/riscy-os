
#include<kernel/string.h>
#include<kernel/stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void memcpy(uint8_t *src, uint8_t *dest, size_t n) {
  // Not ideal but functional
  for(size_t i = 0; i < n; i++) {
    dest[i] = src[i];
  }
}

int strlen(char *c) 
{
  int count = 0;
  while(*c) {
    count++;
    c++;
  }
  return count;
}

int strcmp(char *a, char *b) 
{
  while(*a != '\0' && *b != '\0') 
  {
    if(*a < *b) {
      return -1;
    }
    if(*a > *b) {
      return 1;
    }
  }
  return 0;
}

#ifdef __cplusplus
}
#endif
