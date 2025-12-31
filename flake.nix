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
            # Try explicit version if standard one fails
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
            echo "║     VitusOS Ares Debug Shell               ║"
            echo "╚════════════════════════════════════════════╝"
            
            # 1. FORCE search for wlroots.pc inside the package directly
            echo "Searching for wlroots.pc in ${pkgs.wlroots}..."
            WLROOTS_PC_PATH=$(find ${pkgs.wlroots} -name "wlroots.pc" 2>/dev/null | head -n 1)
            
            if [ -z "$WLROOTS_PC_PATH" ]; then
                echo "CRITICAL FAILURE: wlroots.pc not found in package!"
                echo "Listing package contents:"
                find ${pkgs.wlroots} -maxdepth 3
            else
                echo "FOUND at: $WLROOTS_PC_PATH"
                # Export the directory containing the .pc file
                export PKG_CONFIG_PATH="$(dirname $WLROOTS_PC_PATH):$PKG_CONFIG_PATH"
                echo "Fixed PKG_CONFIG_PATH."
            fi

            echo ""
            echo "Verifying:"
            echo "wlroots: $(pkg-config --modversion wlroots 2>/dev/null || echo 'STILL MISSING')"
            echo ""
            
            export CC=gcc
            export CXX=g++
          '';
        };
      }
    );
}
