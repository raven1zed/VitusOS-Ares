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
            
            # Show what's in the wlroots pkgconfig directory
            echo "Contents of wlroots pkgconfig directory:"
            ls -la ${pkgs.wlroots}/lib/pkgconfig/
            echo ""
            
            # Add wlroots pkgconfig to path
            export PKG_CONFIG_PATH="${pkgs.wlroots}/lib/pkgconfig:${pkgs.wayland}/lib/pkgconfig:${pkgs.libxkbcommon}/lib/pkgconfig:${pkgs.libdrm}/lib/pkgconfig:${pkgs.pixman}/lib/pkgconfig:$PKG_CONFIG_PATH"
            
            # Check what pkg-config can see
            echo "Available wlroots-related packages:"
            pkg-config --list-all 2>/dev/null | grep -i wlr || echo "  None found"
            echo ""
            
            export CC=clang
            export CXX=clang++
          '';
        };
      }
    );
}
