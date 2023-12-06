#!/bin/bash

PWD="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export PATH="$PWD/local/bin:$PATH"

echo
riscv64-elf-gcc --version
echo

echo
riscv64-elf-ld --version
echo

echo
riscv64-elf-ar --version
echo

echo
riscv64-elf-gdb --version
