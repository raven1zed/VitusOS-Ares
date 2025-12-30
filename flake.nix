{
  description = "VitusOS Ares - openSEF Desktop Environment";

  inputs = {
    # Using unstable for wlroots availability
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        
        # === Build Dependencies ===
        buildDeps = with pkgs; [
          cmake
          ninja
          pkg-config
        ];
        
        runtimeDeps = with pkgs; [
          # Wayland
          wayland
          wayland-protocols
          wayland.dev
          libxkbcommon
          libxkbcommon.dev
          # wlroots compositor - IMPORTANT: need both runtime and dev
          wlroots
          # Dev packages for headers and pkg-config
          libdrm
          libdrm.dev
          libinput
          libinput.dev
          pixman
          seatd
          seatd.dev
          # Graphics
          libGL
          mesa
          mesa.dev
          # XWayland
          xwayland
          xorg.libX11
          xorg.libxcb
          xorg.xcbutilwm
          # Fonts
          fontconfig
          fontconfig.dev
          freetype
          freetype.dev
          inter
          dejavu_fonts
        ];
        
        # === openSEF Compositor Package ===
        opensef-compositor = pkgs.stdenv.mkDerivation {
          pname = "opensef-compositor";
          version = "0.1.0";
          
          src = ./opensef/opensef-compositor;
          
          nativeBuildInputs = buildDeps;
          buildInputs = runtimeDeps;
          
          cmakeFlags = [
            "-DCMAKE_BUILD_TYPE=Release"
          ];
          
          meta = with pkgs.lib; {
            description = "openSEF Wayland Compositor for VitusOS Ares";
            license = licenses.mit;
            platforms = platforms.linux;
          };
        };
        
      in {
        # === Packages ===
        packages = {
          inherit opensef-compositor;
          default = opensef-compositor;
        };
        
        # === Development Shell ===
        devShells.default = pkgs.mkShell {
          name = "opensef-dev";
          
          buildInputs = with pkgs; buildDeps ++ runtimeDeps ++ [
            # Compiler
            clang
            llvm
            lld
            gcc
            gdb
            
            # Development tools
            clang-tools     # clangd, clang-format
            bear            # Generate compile_commands.json
            cmake-language-server
            
            # Vulkan (optional)
            vulkan-loader
            vulkan-headers
            
            # FFI
            libffi
            
            # Tools
            git
          ];

          # CRITICAL: Set PKG_CONFIG_PATH so pkg-config can find wlroots
          PKG_CONFIG_PATH = pkgs.lib.makeSearchPath "lib/pkgconfig" (with pkgs; [
            wlroots
            wayland
            wayland.dev
            libxkbcommon
            libxkbcommon.dev
            libdrm
            libdrm.dev
            libinput
            libinput.dev
            pixman
            seatd
            seatd.dev
            libGL
            mesa
            mesa.dev
          ]);

          shellHook = ''
            echo "╔════════════════════════════════════════════╗"
            echo "║     VitusOS Ares Dev Shell                 ║"
            echo "║     openSEF Compositor Environment         ║"
            echo "╚════════════════════════════════════════════╝"
            echo ""
            echo "Build commands:"
            echo "  cd opensef/opensef-compositor"
            echo "  mkdir -p build && cd build"
            echo "  cmake .. -G Ninja"
            echo "  ninja"
            echo ""
            echo "Run compositor:"
            echo "  WLR_BACKENDS=wayland ./opensef-compositor"
            echo ""
            
            # Verify wlroots is found
            if pkg-config --exists wlroots; then
              echo "✓ wlroots found: $(pkg-config --modversion wlroots)"
            else
              echo "✗ wlroots NOT found - check PKG_CONFIG_PATH"
            fi
            echo ""
            
            export CC=clang
            export CXX=clang++
            export FONTCONFIG_PATH=${pkgs.fontconfig.out}/etc/fonts
          '';
        };
      }
    );
}
