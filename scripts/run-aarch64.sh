#!/bin/bash

set -euo pipefail

PWD="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

qemu-system-aarch64 \
    -machine virt \
    -cpu cortex-a57 \
    -kernel "$PWD/../build/riscy-os.elf" \
    -serial stdio \
    -m 1G \
    "$@"
