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
            wayland-scanner
            
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
            
            # Missing deps (from earlier errors)
            libffi
            libcap
            libudev-zero
            libxcb
            xorg.libXau
            xorg.libXdmcp
            
            # XWayland
            xwayland
            xorg.libX11
            xorg.libxcb
            xorg.xcbutilwm
            xorg.xcbutilrenderutil
            xorg.xcbutilimage
            xorg.xcbutilerrors
            
            # Fonts (for future UI)
            fontconfig
            freetype
            
            # Cairo C++ UI (custom shell)
            cairo
            pango
            librsvg  # For SVG icons

            
            # Compiler
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
            echo "║     Pure C Compositor + Cairo/Pango UI     ║"
            echo "╚════════════════════════════════════════════╝"
            echo ""
            
            # Set PKG_CONFIG_PATH for all dependencies
            export PKG_CONFIG_PATH="${pkgs.wlroots}/lib/pkgconfig:${pkgs.wayland}/lib/pkgconfig:${pkgs.libxkbcommon}/lib/pkgconfig:${pkgs.libdrm}/lib/pkgconfig:${pkgs.pixman}/lib/pkgconfig:${pkgs.libinput}/lib/pkgconfig:${pkgs.seatd}/lib/pkgconfig:${pkgs.libxcb}/lib/pkgconfig:${pkgs.libcap}/lib/pkgconfig:${pkgs.libffi}/lib/pkgconfig:${pkgs.xorg.libXau}/lib/pkgconfig:${pkgs.wayland-protocols}/share/pkgconfig:${pkgs.cairo}/lib/pkgconfig:${pkgs.pango}/lib/pkgconfig:${pkgs.librsvg}/lib/pkgconfig:$PKG_CONFIG_PATH"
            
            echo "wlroots: $(pkg-config --modversion wlroots 2>/dev/null || echo 'not found')"
            echo ""
            echo "Build compositor:"
            echo "  cd opensef/opensef-compositor"
            echo "  rm -rf build && mkdir build && cd build"
            echo "  cmake .. -G Ninja && ninja"
            echo ""
            
            export CC=gcc
            export CXX=g++
          '';
        };
      }
    );
}
