#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"

#ifdef KERNEL
#  include <print.h>
#endif

[[noreturn]] void
abort()
{
#ifdef KERNEL
    printk("KERNEL PANIC\n");
#else
    printf("ABORT\n");
#endif

    for (;;) {} /* loop forever */
    unreachable();
}
