#!/bin/bash

set -euo pipefail
shopt -s extglob
shopt -s nullglob
shopt -s globstar

PWD="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT="$( realpath "$PWD/.." )"

clang-format -i \
    "$ROOT"/arch/**/*.{c,cpp,h,hpp} \
    "$ROOT"/include/**/*.{c,cpp,h,hpp} \
    "$ROOT"/drivers/**/*.{c,cpp,h,hpp} \
    "$ROOT"/kernel/**/*.{c,cpp,h,hpp} \
    "$ROOT"/libc/**/*.{c,cpp,h,hpp}
