{
  description = "simulator template";

  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, flake-utils, nixpkgs }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };

        buildInputs = with pkgs; [
          xorg.libX11
          libGL
        ];

        nativeBuildInputs = with pkgs; [
          pkg-config
        ];

        name = "simulator-template";
      in
      rec {
        devShells.default = pkgs.mkShell {
          inherit buildInputs nativeBuildInputs;
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
