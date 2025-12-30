{
  description = "VitusOS Ares - openSEF Desktop Environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    # Pin to a specific commit that has wlroots 0.17
    nixpkgs-wlroots17.url = "github:NixOS/nixpkgs/nixos-23.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, nixpkgs-wlroots17, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        # Use wlroots from 23.11 which has 0.17
        pkgs17 = nixpkgs-wlroots17.legacyPackages.${system};
      in {
        devShells.default = pkgs.mkShell {
          name = "opensef-dev";
          
          packages = with pkgs; [
            # Build tools
            cmake
            ninja
            pkg-config
            wayland-scanner
            
            # Use wlroots 0.17 from older nixpkgs (C++ compatible)
            pkgs17.wlroots
            
            # Core Wayland
            wayland
            wayland-protocols
            libxkbcommon
            libdrm
            libinput
            pixman
            seatd
            libGL
            mesa
            
            # Missing deps
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
            echo "║     Using wlroots 0.17 (C++ compatible)    ║"
            echo "╚════════════════════════════════════════════╝"
            echo ""
            
            # Set PKG_CONFIG_PATH for wlroots 0.17
            export PKG_CONFIG_PATH="${pkgs17.wlroots}/lib/pkgconfig:${pkgs.wayland}/lib/pkgconfig:${pkgs.libxkbcommon}/lib/pkgconfig:${pkgs.libdrm}/lib/pkgconfig:${pkgs.pixman}/lib/pkgconfig:${pkgs.libinput}/lib/pkgconfig:${pkgs.seatd}/lib/pkgconfig:${pkgs.libxcb}/lib/pkgconfig:${pkgs.libcap}/lib/pkgconfig:${pkgs.xorg.libXau}/lib/pkgconfig:${pkgs.wayland-protocols}/share/pkgconfig:$PKG_CONFIG_PATH"
            
            # Check wlroots version
            echo "wlroots version: $(pkg-config --modversion wlroots 2>/dev/null || echo 'not found')"
            echo ""
            echo "Build: cd opensef/opensef-compositor && rm -rf build && mkdir build && cd build && cmake .. -G Ninja && ninja"
            echo ""
            
            export CC=gcc
            export CXX=g++
          '';
        };
      }
    );
}
