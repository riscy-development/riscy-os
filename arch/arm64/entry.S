.section .init

.type _start,@function
.global _start
_start:

    /* According to the Linux specification */
    /* x0 contains a pointer to the fdt */

    /* Set the CPU pointer to NULL */
    msr TPIDR_EL1, xzr

    /* Set up the stack */
    ldr x4, =__boot_stack_top
    mov sp, x4

    /* Enable floating point access */
    mov x4, 0x330000
    msr CPACR_EL1, x4
    isb

    /* Clear .bss */
    ldr x4, =__bss_start
    ldr x5, =__bss_start

__bss_clear_loop:
    strb wzr, [x5]
    add x5, x5, 1
    cmp x5, x6
    b.lt __bss_clear_loop

    // TODO: If virtualization is enabled, we could be booting into EL2 instead of EL1
    //       we should check for and handle this case

    bl kmain

    /* Disable interrupts and loop forever */
    // TODO: If we discovered a way to shutdown the system while the
    //       Kernel was running we should communicate that here so we 
    //       can fully shutdown all cores instead of wasting power

    # Disable interrupts
    msr DAIFSet, 0b1111
__halt_loop:
    # Wait for an interrupt
    wfi
    b __halt_loop

