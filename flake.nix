{
  description = "2 fruity 4 you game";

  inputs.flake-utils.url = "github:numtide/flake-utils";
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  inputs.nixpkgs-doxygen.url = "github:NixOS/nixpkgs/cef50cde2bd337c795159e8b26c7c246206b6740";

  outputs = { self, flake-utils, nixpkgs, nixpkgs-doxygen }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        pkgs-doxygen = import nixpkgs-doxygen { inherit system; };

        buildInputs = with pkgs; [
          xorg.libX11
          libGL
        ];

        nativeBuildInputs = with pkgs; [
          pkg-config
        ];

        name = "game";
      in
      rec {
        devShells.default = pkgs.mkShell {
          inherit buildInputs nativeBuildInputs;
          packages = with pkgs; [
            pkgs-doxygen.doxygen # doxygen 1.9.4
          ];
        };

        packages.default = pkgs.stdenv.mkDerivation rec {
          inherit name buildInputs nativeBuildInputs;
          src = ./.;
          installPhase = ''
            mkdir -p $out/bin
            cp game.out $out/bin/${name}
          '';
        };

        packages.${name} = packages.default;
      }
    );
}