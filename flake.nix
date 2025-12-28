{
  description = "VitusOS Ares - openSEF Development Environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
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
            clang_16
            llvm_16
            lld_16
            
            # Build system
            cmake
            ninja
            pkg-config
            gnumake
            
            # GNUStep + libobjc2
            gnustep.libobjc
            gnustep.base
            gnustep.gui
            gnustep.back
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
            vulkan-validation-layers
            shaderc
            
            # Font rendering (Inter font for Ares theme)
            fontconfig
            freetype
            inter
            
            # Dev tools
            git
            gdb
            valgrind
            bear  # For compile_commands.json
          ];

          # Required for GNUStep
          GNUSTEP_MAKEFILES = "${pkgs.gnustep.make}/share/GNUstep/Makefiles";
          
          shellHook = ''
            echo ""
            echo "╔════════════════════════════════════════════════════════╗"
            echo "║       VitusOS Ares - openSEF Development Shell         ║"
            echo "║                                                        ║"
            echo "║  Theme:   Ares (The Martian)                           ║"
            echo "║  openSEF: Open SeagrEnv Framework                      ║"
            echo "║  Wayland: $(pkg-config --modversion wayland-client 2>/dev/null || echo 'available')"
            echo "║  Vulkan:  Available                                    ║"
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
            
            # Source GNUStep environment if available
            if [ -f "${pkgs.gnustep.make}/share/GNUstep/Makefiles/GNUstep.sh" ]; then
              source "${pkgs.gnustep.make}/share/GNUstep/Makefiles/GNUstep.sh"
            fi
          '';
        };
      }
    );
}
