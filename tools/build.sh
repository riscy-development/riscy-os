#!/bin/bash

set -euo pipefail

PWD="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BIN="$PWD/local/bin"

rm -rf "$PWD/local"
mkdir -p "$PWD/local/bin"

pushd() {
    command pushd $@ > /dev/null
}

popd() {
    command popd $@ > /dev/null
}

#
# Kbuild standalone
#
pushd "$PWD/kbuild-standalone"

rm -rf build
mkdir -p build

pushd build

make -C ../ -f Makefile.sample O=`pwd` -j$(nproc)

ln -s "$(pwd)/kconfig/conf" "$BIN/kbuild-conf"
ln -s "$(pwd)/kconfig/mconf" "$BIN/kbuild-mconf"
ln -s "$(pwd)/fixdep/fixdep" "$BIN/kbuild-fixdep"
ln -s "$(pwd)/unifdef/unifdef" "$BIN/kbuild-unifdef"

popd # build

popd # kbuild-standalone
