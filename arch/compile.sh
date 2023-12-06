#!/bin/bash

export PATH="/home/nikola/code/riscy-os/toolchain/local/bin:$PATH"

riscv64-elf-gcc -Wall -Wextra -c -mcmodel=medany kernel.c -o kernel.o -ffreestanding
riscv64-elf-as -c entry.S -o entry.o
riscv64-elf-ld -T linker.ld -nostdlib kernel.o entry.o -o kernel.elf
