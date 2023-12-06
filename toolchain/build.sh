#!/bin/bash

set -euo pipefail

#######################################################################################
### Configuration
#######################################################################################
BINUTILS_VERSION="2.41"
GCC_VERSION="13.2.0"
GDB_VERSION="14.1"

#######################################################################################
### Setup
#######################################################################################

# Check arguments
if [[ $# -eq 1 ]]; then
    ARCH=$1
else
    echo "Usage: $0 <arch>"
    exit 1
fi

# Get working directory
PWD="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT="$( realpath "$PWD/.." )"

# Create directories for src and bin
SRC="$PWD/src"
BIN="$PWD/local"
BUILD="$PWD/build/$ARCH"

mkdir -p "$SRC"
mkdir -p "$BIN"
mkdir -p "$BUILD"

# Get message binary
MESSAGE="figlet"

msg() {
    echo "--------------------------------------------------------------------------------"
    $MESSAGE $@
    echo "--------------------------------------------------------------------------------"
    echo
}

# See if we can exit early
if [[ -f "$BIN/bin/$ARCH-elf-gcc" ]]; then
    msg "Toolchain  ready!"
    exit 0
fi

#######################################################################################
### Downloads
#######################################################################################
msg "Downloading Sources"

# Download helper
# download <name> <tarball> <url>
download() {
    pushd $SRC > /dev/null
    
    # Arguments
    name="$1"
    tarball="$2"
    src_dir="$3"
    url="$4"

    # Message
    echo "Downloading $name"

    # Download
    if ! [[ -f "$tarball" ]]; then
        echo "    Downloading tarball from gnu.org"
        wget -q "$url" -O "$tarball"
    else
        echo "    Tarball already exits, skipping"
    fi

    # Extract
    if ! [[ -d "$src_dir" ]]; then
        echo "    Extracting tarball..."
        tar -xf "$tarball"
    else
        echo "    Source directory exists, skipping"
    fi

    # Finish
    echo -e "Done!\n"
    popd > /dev/null
}


# Binutils
BINUTILS_TARBALL="binutils-${BINUTILS_VERSION}.tar"
BINUTILS_SRC_DIR="binutils-${BINUTILS_VERSION}"

download binutils \
         $BINUTILS_TARBALL \
         $BINUTILS_SRC_DIR \
         "ftp://ftp.gnu.org/gnu/binutils/${BINUTILS_TARBALL}.xz"

# GDB
GDB_TARBALL="gdb-${GDB_VERSION}.tar"
GDB_SRC_DIR="gdb-${GDB_VERSION}"

download gcc \
         $GDB_TARBALL \
         $GDB_SRC_DIR \
         "ftp://ftp.gnu.org/gnu/gdb/${GDB_TARBALL}.xz"

# GCC
GCC_TARBALL="gcc-${GCC_VERSION}.tar"
GCC_SRC_DIR="gcc-${GCC_VERSION}"

download gcc \
         $GCC_TARBALL \
         $GCC_SRC_DIR \
         "ftp://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/${GCC_TARBALL}.xz"

#######################################################################################
### Building
#######################################################################################
do-make() {
    make -j$(nproc) $@ || exit 1
}

# Set targets
TARGET="$ARCH-elf"
PREFIX="$BIN"

echo "Installing to $PREFIX"

# Enter build dir
pushd "$BUILD" > /dev/null

# Build binutils
msg "Building binutils"

mkdir -p binutils
pushd binutils

"$SRC/$BINUTILS_SRC_DIR/configure" \
    --prefix="$PREFIX" \
    --target="$TARGET" \
    --with-sysroot \
    --enable-shared \
    --disable-nls \
    --disable-werror || exit 1

do-make
do-make install

echo -e "DONE\n"
popd

# Build GDB
msg "Building GDB"

mkdir -p gdb
pushd gdb

"$SRC/$GDB_SRC_DIR/configure" \
    --prefix="$PREFIX" \
    --target="$TARGET" \
    --with-sysroot \
    --enable-shared \
    --disable-nls \
    --disable-werror || exit 1

do-make all-gdb
do-make install-gdb install-opcodes

echo -e "DONE\n"
popd

# Build GCC
msg "Building GCC"

mkdir -p gcc
pushd gcc

"$SRC/$GCC_SRC_DIR/configure" \
    --prefix="$PREFIX" \
    --target="$TARGET" \
    --with-sysroot \
    --disable-bootstrap \
    --enable-shared \
    --disable-nls \
    --with-newlib \
    --disable-werror \
    --enable-languages="c,c++" || exit 1

do-make all-gcc
do-make all-target-libgcc
do-make install-gcc install-target-libgcc 

popd

popd
