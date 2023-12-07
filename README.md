# RISC-y OS

A OS for RISC-V. Use at your own RISC.

## Building

Make sure you have Nix installed. Then run the following

```sh
$ git submodule update --init --recursive
$ git submodule update --remote
$ nix-shell
$ make
```

Follow configuration menu, and save. 

It will take a while to build the toolchain.
