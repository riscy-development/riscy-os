#!/bin/bash

PWD="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

rm -rf "$PWD/kbuild-standalone/build"
rm -rf "$PWD/local"
