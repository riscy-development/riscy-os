
#include <stdio.h>
#include <string.h>

static int formatted_print(void(*puts)(const char*), const char *fmt, va_list args) 
{
#define BUF_LEN 32
_Static_assert(BUF_LEN > 2, "formatted_print BUF_LEN too small!");

  int num_chars = 0;

  char buf[BUF_LEN+1];
  int buf_head = 0;

  // Add backup null terminator
  buf[BUF_LEN] = '\0'

  bool escaped = false;
  for(size_t i = 0; i < strlen(fmt); i++) 
  {
    if(escaped) {
      // Flush the buffer no matter what
      if(buf_head > 0) {
        buf[buf_head] = '\0';
        (*puts)(buf);
        num_chars += buf_head;
        buf_head = 0;
      }

      switch(*fmt) {
        case '%':
          buf[0] = '%';
          buf[1] = '\0';
          (*puts)(buf);
          num_chars += 1;
          break;
        case 'c':
          buf[0] = va_arg(args, char);
          buf[1] = '\0'
          (*puts)(buf);
          num_chars += 1;
          break;
        default:
          buf[0] = '%';
          buf[1] = *fmt;
          buf[2] = '\0';
          num_chars += 2;
          (*puts)(buf);
          break;
      }
    }

    if(!escaped) {
      if(*fmt == '%') {
        escaped = true;
      } else {
        buf[buf_head] = *fmt;
        buf_head++;

        // Flush the buffer if needed
        if(buf_head == BUF_LEN) {
          (*puts)(buf);
          num_chars += BUF_LEN;
          buf_head = 0;
        }
      }
    }
  }

  return num_chars;
}

void printf_puts(const char *s) {
  // TODO
}

int printf(const char *fmt, ...) 
{
  va_list args;
  va_begin(args,fmt);
  int num_chars = formatted_print(printf_puts,fmt,args);
  va_end(args);
  return num_chars;
}

#ifdef KERNEL

#include <kernel/early_output.h>

static void printk_puts(const char *s) 
{
  early_puts(s);
}

int printk(const char *fmt, ...) 
{
  va_list args;
  va_begin(args,fmt);
  int num_chars = formatted_print(printk_puts,fmt,args);
  va_end(args);
  return num_chars;
}
#endif

