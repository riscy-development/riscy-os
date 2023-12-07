.section .init
 
.option norvc  /* no compressed instructions */

.type _start, @function
.global _start
_start:
        .cfi_startproc

        /* According to the Linux specification: */
        /* a0 contains the hartid */
	/* a1 contains a pointer to the fdt */

        /* Set up global pointer */
        .option push
        .option norelax
        la gp, __global_pointer$
        .option pop

        /* Reset satp */
        csrw satp, zero

        /* Setup stack */
        la sp, __boot_stack_top

        /* Clear the BSS section */
        la t5, __bss_start
        la t6, __bss_end

__bss_clear_loop:
        sd zero, (t5)
        addi t5, t5, 8
        bltu t5, t6, __bss_clear_loop

        /* Machine exception PC */
        /* Set to executing mret jumps to main */
        la t0, kmain
        csrw mepc, t0

        /* If kmain returns, we're done */
        la ra, halt_forever

        /* Jump to kernel! */
        jr t0

        .cfi_endproc

/* Done with everything */
.type halt_forever, @function
halt_forever:
        .cfi_startproc

        csrw mie, zero
        wfi
        j halt_forever

        .cfi_endproc

.end
