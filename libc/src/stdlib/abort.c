#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"

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
