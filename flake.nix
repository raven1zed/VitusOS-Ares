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
        
        # === All Dependencies (simplified - no .dev suffixes that don't exist) ===
        allDeps = with pkgs; [
          # Build tools
          cmake
          ninja
          pkg-config
          
          # Wayland core
          wayland
          wayland-protocols
          libxkbcommon
          
          # wlroots compositor
          wlroots
          
          # System libs
          libdrm
          libinput
          pixman
          seatd
          
          # Graphics
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
          inter
          dejavu_fonts
        ];
        
      in {
        # === Development Shell ===
        devShells.default = pkgs.mkShell {
          name = "opensef-dev";
          
          buildInputs = allDeps ++ (with pkgs; [
            # Compiler
            clang
            llvm
            lld
            gcc
            gdb
            
            # Dev tools
            clang-tools
            bear
            cmake-language-server
            
            # Vulkan
            vulkan-loader
            vulkan-headers
            
            libffi
            git
          ]);

          shellHook = ''
            echo "╔════════════════════════════════════════════╗"
            echo "║     VitusOS Ares Dev Shell                 ║"
            echo "║     openSEF Compositor Environment         ║"
            echo "╚════════════════════════════════════════════╝"
            echo ""
            
            # Set PKG_CONFIG_PATH for wlroots
            export PKG_CONFIG_PATH="${pkgs.wlroots}/lib/pkgconfig:${pkgs.wayland}/lib/pkgconfig:${pkgs.libxkbcommon}/lib/pkgconfig:${pkgs.libdrm}/lib/pkgconfig:${pkgs.libinput}/lib/pkgconfig:${pkgs.pixman}/lib/pkgconfig:${pkgs.seatd}/lib/pkgconfig:${pkgs.libGL}/lib/pkgconfig:${pkgs.mesa}/lib/pkgconfig:$PKG_CONFIG_PATH"
            
            if pkg-config --exists wlroots; then
              echo "✓ wlroots found: $(pkg-config --modversion wlroots)"
            else
              echo "✗ wlroots NOT found"
            fi
            echo ""
            echo "Build: cd opensef/opensef-compositor && mkdir -p build && cd build && cmake .. -G Ninja && ninja"
            echo ""
            
            export CC=clang
            export CXX=clang++
          '';
        };
      }
    );
}
