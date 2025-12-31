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
            gnumake
            git
          ];
          
          buildInputs = with pkgs; [
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
            libffi
            libcap
            udev
            libxcb
            xwayland
            cairo
            pango
            librsvg
            glib
            fontconfig
            freetype
          ];

          shellHook = ''
            # Manually add wlroots to PKG_CONFIG_PATH
            export PKG_CONFIG_PATH="${pkgs.wlroots}/lib/pkgconfig:$PKG_CONFIG_PATH"
            
            echo "╔════════════════════════════════════════════╗"
            echo "║     VitusOS Ares Dev Shell                 ║"
            echo "║     Pure C Compositor + Cairo/Pango UI     ║"
            echo "╚════════════════════════════════════════════╝"
            echo ""
            echo "wlroots: $(pkg-config --modversion wlroots 2>/dev/null || echo 'NOT FOUND')"
            echo "cairo:   $(pkg-config --modversion cairo 2>/dev/null || echo 'NOT FOUND')"
            echo ""
            
            export CC=gcc
            export CXX=g++
          '';
        };
      }
    );
}
