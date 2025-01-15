{
  description = "Godost development environment";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (
    system:
    let
      pkgs = import nixpkgs {
        inherit system;
      };
    in
    with pkgs;
    {
      devShells.default = mkShell {
          buildInputs =  [
            cargo
            rustup
            fish
            just
          ];
          shellHook = ''
            exec fish
          '';
      };    
    }
  );
}
