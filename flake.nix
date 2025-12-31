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
          
          nativeBuildInputs = with pkgs; [
            # Build tools
            cmake
            ninja
            pkg-config
            wayland-scanner
            gcc
            gdb
            clang-tools
            bear
            git
            gnumake
          ];
          
          buildInputs = with pkgs; [
            # Core Wayland/wlroots (with dev headers)
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
            
            # Additional deps
            libffi
            libcap
            udev
            libxcb
            xorg.libXau
            xorg.libXdmcp
            
            # XWayland
            xwayland
            xorg.libX11
            xorg.xcbutilwm
            xorg.xcbutilrenderutil
            xorg.xcbutilimage
            xorg.xcbutilerrors
            
            # Cairo/Pango for UI
            cairo
            pango
            librsvg
            glib
            
            # Fonts
            fontconfig
            freetype
          ];

          shellHook = ''
            echo "╔════════════════════════════════════════════╗"
            echo "║     VitusOS Ares Dev Shell                 ║"
            echo "║     Pure C Compositor + Cairo/Pango UI     ║"
            echo "╚════════════════════════════════════════════╝"
            echo ""
            echo "wlroots: $(pkg-config --modversion wlroots 2>/dev/null || echo 'not found')"
            echo "cairo:   $(pkg-config --modversion cairo 2>/dev/null || echo 'not found')"
            echo ""
            echo "Build:"
            echo "  cd opensef && mkdir -p build && cd build"
            echo "  cmake .. -G Ninja && ninja"
            echo ""
            
            export CC=gcc
            export CXX=g++
          '';
        };
      }
    );
}
