#!/bin/bash

PWD="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT="$( realpath "$PWD/.." )"

export PATH="$ROOT/tools/local/bin:$ROOT/toolchain/local/bin:$PATH"
