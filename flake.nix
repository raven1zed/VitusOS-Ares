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
          libxkbcommon
          # wlroots compositor
          wlroots
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
            echo "Build commands:"
            echo "  cd opensef/opensef-compositor"
            echo "  mkdir -p build && cd build"
            echo "  cmake .. -G Ninja"
            echo "  ninja"
            echo ""
            echo "Run compositor:"
            echo "  ./build/opensef-compositor"
            echo ""
            
            export CC=clang
            export CXX=clang++
            export FONTCONFIG_PATH=${pkgs.fontconfig.out}/etc/fonts
          '';
        };
      }
    );
}

