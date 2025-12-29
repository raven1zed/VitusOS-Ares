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
            inter            # Inter font!
            dejavu_fonts     # Fallback font
            
            # FFI
            libffi
            
            # Tools
            git
          ];

          shellHook = ''
            echo "╔════════════════════════════════════════════╗"
            echo "║     VitusOS Ares Dev Shell                 ║"
            echo "║     With Inter Font & FreeType             ║"
            echo "╚════════════════════════════════════════════╝"
            export CC=clang
            export CXX=clang++
            export FONTCONFIG_PATH=${pkgs.fontconfig.out}/etc/fonts
          '';
        };
      }
    );
}
