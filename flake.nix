{
  description = "VitusOS Ares - openSEF Desktop Environment (Fixed for NixOS Unstable)";

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
            echo "╔════════════════════════════════════════════╗"
            echo "║     VitusOS Ares Dev Shell                 ║"
            echo "╚════════════════════════════════════════════╝"
            
            # Fix PKG_CONFIG_PATH explicitly for split outputs (unstable channel quirks)
            export PKG_CONFIG_PATH="${pkgs.wlroots}/lib/pkgconfig:${pkgs.wayland.dev}/lib/pkgconfig:${pkgs.wayland-protocols}/share/pkgconfig:${pkgs.cairo.dev}/lib/pkgconfig:${pkgs.pango.dev}/lib/pkgconfig:${pkgs.libxkbcommon.dev}/lib/pkgconfig:${pkgs.mesa}/lib/pkgconfig:$PKG_CONFIG_PATH"

            echo "Detecting wlroots version..."
            if pkg-config --exists wlroots-0.19; then
                echo "✅ Found wlroots-0.19"
            elif pkg-config --exists wlroots; then
                echo "✅ Found wlroots (generic)"
            else
                echo "⚠️  Note: 'wlroots' alias not found, but 'wlroots-0.19' likely exists."
                echo "    CMake has been updated to find 0.19 automatically."
            fi
            
            export CC=gcc
            export CXX=g++
          '';
        };
      }
    );
}
