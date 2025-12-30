{
  description = "VitusOS Ares - openSEF Desktop Environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in {
        devShells.default = pkgs.mkShell {
          name = "opensef-dev";
          
          packages = with pkgs; [
            # Build tools
            cmake
            ninja
            pkg-config
            
            # Core Wayland/wlroots
            wlroots
            wayland
            wayland-protocols
            libxkbcommon
            libdrm
            libinput
            pixman
            seatd
            libGL
            mesa
            
            # XWayland
            xwayland
            xorg.libX11
            xorg.libxcb
            xorg.xcbutilwm
            
            # Fonts
            fontconfig
            freetype
            
            # Compiler
            clang
            gcc
            gdb
            
            # Dev tools
            clang-tools
            bear
            git
          ];

          shellHook = ''
            echo "╔════════════════════════════════════════════╗"
            echo "║     VitusOS Ares Dev Shell                 ║"
            echo "╚════════════════════════════════════════════╝"
            echo ""
            
            # Find where wlroots.pc actually is
            WLROOTS_PC=$(find ${pkgs.wlroots} -name "wlroots.pc" 2>/dev/null | head -1)
            
            if [ -n "$WLROOTS_PC" ]; then
              WLROOTS_PKG_DIR=$(dirname "$WLROOTS_PC")
              echo "✓ Found wlroots.pc at: $WLROOTS_PC"
              export PKG_CONFIG_PATH="$WLROOTS_PKG_DIR:$PKG_CONFIG_PATH"
            else
              echo "✗ wlroots.pc not found in ${pkgs.wlroots}"
              echo "  Contents of wlroots package:"
              ls -la ${pkgs.wlroots}/lib/ 2>/dev/null || echo "  No lib directory"
            fi
            
            # Also add standard paths
            export PKG_CONFIG_PATH="${pkgs.wayland}/lib/pkgconfig:${pkgs.libxkbcommon}/lib/pkgconfig:${pkgs.libdrm}/lib/pkgconfig:${pkgs.pixman}/lib/pkgconfig:$PKG_CONFIG_PATH"
            
            # Test
            if pkg-config --exists wlroots; then
              echo "✓ pkg-config finds wlroots: $(pkg-config --modversion wlroots)"
            else
              echo "✗ pkg-config cannot find wlroots"
              echo "  Current PKG_CONFIG_PATH:"
              echo "  $PKG_CONFIG_PATH" | tr ':' '\n' | head -5
            fi
            echo ""
            
            export CC=clang
            export CXX=clang++
          '';
        };
      }
    );
}
