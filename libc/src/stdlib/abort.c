#include "stddef.h"
#include "stdlib.h"

#ifdef KERNEL
#  include <print.h>
#endif

[[noreturn]] void
abort()
{
#ifdef KERNEL
    early_puts("KERNEL PANIC");
#else
/* TODO print something */
#endif

    for (;;) {} /* loop forever */
    unreachable();
}
