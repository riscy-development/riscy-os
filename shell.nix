let
  nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/tarball/nixos-23.11";
  pkgs = import nixpkgs { config = {}; overlays = []; };
in

pkgs.mkShell {
  packages = with pkgs; [
    wget figlet

    # GCC dependencies
    coreutils moreutils binutils
    bison flex
    gmp mpfr libmpc zlib
    texinfo gengetopt

    # bash
    bashInteractive
  ];

  hardeningDisable = [ "all" ];
}
