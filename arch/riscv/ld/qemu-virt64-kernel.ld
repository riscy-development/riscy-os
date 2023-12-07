/* Entry symbol is _start */
ENTRY(_start);

/* Start of RAM in RISC-V */
. = 0x80000000;
 
SECTIONS {
    /**
     * ELF file layout: text section at bottom, followed by data and bss
     * sections
     *
     * Global pointer starts where BSS ends
     * The heap also starts right after the BSS section, growing upwards
     * The stack grows downwards, starting from the end of RAM
     * This mean there's a chance stack and heap collide, but we'll assume
     * it never happens
     */

    /* First is our init section, containing our entrypoint. */
    .init : ALIGN(4K) {
        *(.init);
    }

    /* Now our text section, with our code. */
    .text : ALIGN(4K) {
        *(.init);
        *(.text);
    }

    /* Then global constructor lists */
    .preinit_array : ALIGN(4K) {
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN(__preinit_array_end = .);
    }
    .init_array : ALIGN(4K) {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP(*(.init_array EXCLUDE_FILE(*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
        PROVIDE_HIDDEN(__init_array_end = .);
    }
    .fini_array : ALIGN(4K) {
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP(*(.fini_array EXCLUDE_FILE(*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
        PROVIDE_HIDDEN(__fini_array_end = .);
    }

    /* Then our data sections */
    .rodata : ALIGN(4K) {
        PROVIDE(__rodata_begin = .);
        *(.rodata);
        PROVIDE(__rodata_end = .);
    }
    .sdata : ALIGN(4K) {
        PROVIDE(__sdata_begin = .);
        *(.sdata);
        PROVIDE(__sdata_end = .);
    }
    .data : ALIGN(4K) {
        PROVIDE(__data_begin = .);
        *(.data);
        PROVIDE(__data_end = .);
    }

    /* And the BSS (zeroed data) section */
    .bss : ALIGN(4K) {
        PROVIDE(__bss_start = .);
        *(.sbss); /* TODO: does this need to go here? */
        *(.bss);
        PROVIDE(__bss_end = .);
    }

    PROVIDE(
        __global_pointer$ = MIN(__sdata_begin + 0x800,
                                MAX(__data_begin + 0x800, __bss_end - 0x800))
    );

    /* Boot stack */
    .boot_stack : ALIGN(4K) {
        PROVIDE_HIDDEN(__boot_stack_bottom = .);
        . += 0x200000;  /* we start out with a 2M stack */
        PROVIDE_HIDDEN(__boot_stack_top = .);
     }

    PROVIDE(__kernel_end = .);
}
