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

          shellHook = ''
            echo "╔════════════════════════════════════════════╗"
            echo "║     VitusOS Ares Dev Shell                 ║"
            echo "║     openSEF Compositor Environment         ║"
            echo "╚════════════════════════════════════════════╝"
            echo ""
            
            # CRITICAL: Set PKG_CONFIG_PATH directly so pkg-config finds wlroots
            export PKG_CONFIG_PATH="${pkgs.wlroots}/lib/pkgconfig:${pkgs.wayland}/lib/pkgconfig:${pkgs.wayland.dev}/lib/pkgconfig:${pkgs.libxkbcommon}/lib/pkgconfig:${pkgs.libxkbcommon.dev}/lib/pkgconfig:${pkgs.libdrm}/lib/pkgconfig:${pkgs.libdrm.dev}/lib/pkgconfig:${pkgs.libinput}/lib/pkgconfig:${pkgs.libinput.dev}/lib/pkgconfig:${pkgs.pixman}/lib/pkgconfig:${pkgs.seatd}/lib/pkgconfig:${pkgs.seatd.dev}/lib/pkgconfig:${pkgs.libGL}/lib/pkgconfig:${pkgs.mesa}/lib/pkgconfig:${pkgs.mesa.dev}/lib/pkgconfig:$PKG_CONFIG_PATH"
            
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
              echo "✗ wlroots NOT found"
              echo "  PKG_CONFIG_PATH=$PKG_CONFIG_PATH"
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
