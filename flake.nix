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
            wlroots
            wlroots.dev  # CRITICAL: dev output contains .pc file
            wayland
            wayland-protocols
            libxkbcommon
            libdrm
            libinput
            pixman
            seatd
            libGL
            mesa
            libffi
            libcap
            udev
            libxcb
            xorg.libXau
            xorg.libXdmcp
            xwayland
            xorg.libX11
            xorg.xcbutilwm
            xorg.xcbutilrenderutil
            xorg.xcbutilimage
            xorg.xcbutilerrors
            cairo
            pango
            librsvg
            glib
            fontconfig
            freetype
          ];

          # CRITICAL: Manually set PKG_CONFIG_PATH for all libs
          PKG_CONFIG_PATH = with pkgs; lib.makeSearchPath "lib/pkgconfig" [
            wlroots
            wayland
            wayland.dev
            libxkbcommon
            libdrm
            libinput
            pixman
            seatd
            libGL
            mesa
            libffi
            libcap
            libxcb
            xorg.libXau
            cairo
            pango
            librsvg
            glib
            fontconfig
            freetype
          ] + ":" + lib.makeSearchPath "share/pkgconfig" [
            wayland-protocols
          ];

          shellHook = ''
            echo "╔════════════════════════════════════════════╗"
            echo "║     VitusOS Ares Dev Shell                 ║"
            echo "║     Pure C Compositor + Cairo/Pango UI     ║"
            echo "╚════════════════════════════════════════════╝"
            echo ""
            echo "wlroots: $(pkg-config --modversion wlroots 2>/dev/null || echo 'NOT FOUND')"
            echo "cairo:   $(pkg-config --modversion cairo 2>/dev/null || echo 'NOT FOUND')"
            echo "wayland: $(pkg-config --modversion wayland-server 2>/dev/null || echo 'NOT FOUND')"
            echo ""
            echo "Build:"
            echo "  cd opensef && rm -rf build && mkdir build && cd build"
            echo "  cmake .. -G Ninja && ninja"
            echo ""
            
            export CC=gcc
            export CXX=g++
          '';
        };
      }
    );
}
