{
  description = "VitusOS Ares - openSEF Development Environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          name = "opensef-dev";
          
          buildInputs = with pkgs; [
            # Compiler
            clang
            llvm
            lld
            
            # Build
            cmake
            ninja
            pkg-config
            
            # Wayland
            wayland
            wayland-protocols
            wayland-scanner
            libxkbcommon
            
            # Vulkan
            vulkan-loader
            vulkan-headers
            
            # Fonts
            fontconfig
            freetype
            
            # Tools
            git
          ];

          shellHook = ''
            echo "VitusOS Ares Dev Shell Ready"
            export CC=clang
            export CXX=clang++
          '';
        };
      }
    );
}
