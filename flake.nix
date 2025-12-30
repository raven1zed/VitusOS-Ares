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
            wayland-scanner  # IMPORTANT: For generating protocol files
            
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
            
            # Missing dependencies from error
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
            
            # Set PKG_CONFIG_PATH for all dependencies
            export PKG_CONFIG_PATH="${pkgs.wlroots}/lib/pkgconfig:${pkgs.wayland}/lib/pkgconfig:${pkgs.libxkbcommon}/lib/pkgconfig:${pkgs.libdrm}/lib/pkgconfig:${pkgs.pixman}/lib/pkgconfig:${pkgs.libinput}/lib/pkgconfig:${pkgs.seatd}/lib/pkgconfig:${pkgs.libxcb}/lib/pkgconfig:${pkgs.libcap}/lib/pkgconfig:${pkgs.xorg.libXau}/lib/pkgconfig:${pkgs.wayland-protocols}/share/pkgconfig:$PKG_CONFIG_PATH"
            
            echo "✓ All dependencies configured"
            echo ""
            echo "Build: cd opensef/opensef-compositor && rm -rf build && mkdir build && cd build && cmake .. -G Ninja && ninja"
            echo ""
            
            # Use GCC instead of clang - clang has issues with wlroots C99 syntax
            export CC=gcc
            export CXX=g++
          '';
        };
      }
    );
}

