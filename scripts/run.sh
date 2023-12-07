#!/bin/bash

set -euo pipefail

PWD="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

qemu-system-riscv64 \
    -machine virt \
    -bios none \
    -kernel "$PWD/../build/riscy-os.elf" \
    -serial mon:stdio
