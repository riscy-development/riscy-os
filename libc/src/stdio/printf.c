
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/*
 * Checklist of formatting functionality
 *
 * %% - [DONE]
 * %c - [DONE]
 * %s
 *
 * TODO:
 * %d
 * %i
 * %e
 * %E
 * %f
 * %g
 * %G
 * %o
 * %u
 * %x
 * %X
 * %p
 * %n
 *
 * Ignoring modifiers for now
 */

static int formatted_print(void(*puts)(const char*), const char *fmt, va_list args) 
{
#define BUF_LEN 32
_Static_assert(BUF_LEN > 2, "formatted_print BUF_LEN too small!");

  int num_chars = 0;

  char buf[BUF_LEN+1];
  int buf_head = 0;

  // Add backup null terminator
  buf[BUF_LEN] = '\0';

  bool escaped = false;
  do 
  {
    if(escaped) {
      // Flush the buffer no matter what
      if(buf_head > 0) {
        buf[buf_head] = '\0';
        (*puts)(buf);
        num_chars += buf_head;
        buf_head = 0;
      }

      // Predefinitions for use in the switch statement
      char *s;
      //

      switch(*fmt) {
        case '\0':
          // In this case we need to output the final %
        case '%':
          buf[0] = '%';
          buf[1] = '\0';
          (*puts)(buf);
          num_chars += 1;
          escaped = false;
          break;
        case 'c':
          buf[0] = (char)va_arg(args, int);
          buf[1] = '\0';
          (*puts)(buf);
          num_chars += 1;
          escaped = false;
          break;
        case 's':
          s = (char*)va_arg(args,char*);
          (*puts)(s);
          num_chars += strlen(s);
          escaped = false;
          break;
        default:
          buf[0] = '%';
          buf[1] = *fmt;
          buf[2] = '\0';
          num_chars += 2;
          (*puts)(buf);
          escaped = false;
          break;
      }
    }

    else if(!escaped && *fmt != '\0') {
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

    fmt++;
  } while(*fmt != '\0');
  
  // Flush the buffer no matter what
  if(buf_head > 0) {
    buf[buf_head] = '\0';
    (*puts)(buf);
    num_chars += buf_head;
    buf_head = 0;
  }

  return num_chars;
}

void printf_puts(const char *s) {
  // TODO
}

#ifdef KERNEL

#include <kernel/early_output.h>

static void printk_puts(const char *s) 
{
  early_puts(s);
}

#endif

int printf(const char *fmt, ...) 
{
  va_list args;
  va_start(args,fmt);
  
  int num_chars = formatted_print(
#ifdef KERNEL
      printk_puts
#else
      printf_puts
#endif
      ,fmt,args);

  va_end(args);

  return num_chars;
}

