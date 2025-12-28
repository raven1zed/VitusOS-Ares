{
  description = "VitusOS Ares - openSEF Development Environment";

  inputs = {
    # Use NixOS 24.05 stable - has GNUstep packages
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
            # Compiler toolchain (Clang required for ObjC 2.0)
            clang
            llvm
            lld
            
            # Build system
            cmake
            ninja
            pkg-config
            gnumake
            
            # GNUStep (available in 24.05 stable)
            gnustep-libobjc
            gnustep.base
            gnustep.make
            
            # Wayland (VitusOS is Wayland-only)
            wayland
            wayland-protocols
            wayland-scanner
            libxkbcommon
            
            # Vulkan rendering
            vulkan-loader
            vulkan-headers
            vulkan-tools
            shaderc
            
            # Font rendering
            fontconfig
            freetype
            
            # Dev tools
            git
            gdb
          ];

          # GNUStep environment
          GNUSTEP_MAKEFILES = "${pkgs.gnustep.make}/share/GNUstep/Makefiles";
          
          shellHook = ''
            echo ""
            echo "╔════════════════════════════════════════════════════════╗"
            echo "║       VitusOS Ares - openSEF Development Shell         ║"
            echo "║                                                        ║"
            echo "║  GNUstep: Available (nixos-24.05)                      ║"
            echo "║  Theme:   Ares (The Martian)                           ║"
            echo "╚════════════════════════════════════════════════════════╝"
            echo ""
            echo "Quick Start:"
            echo "  cmake -B build -G Ninja"
            echo "  cmake --build build"
            echo ""
            
            export CC=clang
            export CXX=clang++
            export OBJC=clang
            export OBJCFLAGS="-fobjc-runtime=gnustep-2.0 -fobjc-arc -fblocks"
            
            # Source GNUStep environment
            if [ -f "${pkgs.gnustep.make}/share/GNUstep/Makefiles/GNUstep.sh" ]; then
              source "${pkgs.gnustep.make}/share/GNUstep/Makefiles/GNUstep.sh"
            fi
          '';
        };
      }
    );
}
