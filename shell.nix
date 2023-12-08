let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/tarball/nixos-23.11";
  pkgs = import nixpkgs { config = {}; overlays = []; };
in

pkgs.mkShell {
  packages = with pkgs; [
    # Script dependencies
    wget figlet
    python312 black

    # Build deps
    cmake

    # Kbuild deps
    ncurses

    # GCC dependencies
    coreutils moreutils binutils
    bison flex
    gmp mpfr libmpc zlib
    texinfo gengetopt

    # Testing dependencies
    qemu scc

    # bash
    bashInteractive
  ];

  hardeningDisable = [ "all" ];
}
