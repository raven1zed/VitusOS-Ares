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
            whitesur-icon-theme
            imagemagick
            clang
            llvmPackages.libcxxClang
            gdb
            gnumake
            git
            weston
            qt6.wrapQtAppsHook
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
            xorg.libxcb
            xwayland
            cairo
            pango
            librsvg
            glib
            fontconfig
            freetype
            vulkan-headers
            vulkan-loader
            vulkan-tools
            vulkan-validation-layers
            glm
            # Qt6 components - explicit packages for shell
            qt6.qtbase
            qt6.qtwayland
            qt6.qtdeclarative
            qt6.qtsvg
            qt6.qt5compat  # For QtGraphicalEffects
            # Additional dependencies
            pam
            sysprof
            libdisplay-info
            libxml2
            pcre2
            util-linux.dev
          ];

          shellHook = ''
            echo "╔════════════════════════════════════════════╗"
            echo "║     VitusOS Ares Dev Shell                 ║"
            echo "╚════════════════════════════════════════════╝"
            
            # Fix PKG_CONFIG_PATH explicitly for split outputs (unstable channel quirks)
            export PKG_CONFIG_PATH="${pkgs.wlroots}/lib/pkgconfig:${pkgs.wayland.dev}/lib/pkgconfig:${pkgs.wayland-protocols}/share/pkgconfig:${pkgs.cairo.dev}/lib/pkgconfig:${pkgs.pango.dev}/lib/pkgconfig:${pkgs.libxkbcommon.dev}/lib/pkgconfig:${pkgs.mesa}/lib/pkgconfig:${pkgs.pam}/lib/pkgconfig:${pkgs.sysprof.dev}/lib/pkgconfig:${pkgs.libxml2.dev}/lib/pkgconfig:${pkgs.pcre2.dev}/lib/pkgconfig:${pkgs.util-linux.dev}/lib/pkgconfig:${pkgs.glib.dev}/lib/pkgconfig:${pkgs.harfbuzz.dev}/lib/pkgconfig:$PKG_CONFIG_PATH"

            echo "Detecting wlroots version..."
            if pkg-config --exists wlroots-0.19; then
                echo "✅ Found wlroots-0.19"
            elif pkg-config --exists wlroots; then
                echo "✅ Found wlroots (generic)"
            else
                echo "⚠️  Note: 'wlroots' alias not found, but 'wlroots-0.19' likely exists."
                echo "    CMake has been updated to find 0.19 automatically."
            fi
            
            export CC=clang
            export CXX=clang++
            echo "Using Clang compiler: $(clang --version | head -n1)"
            
            # Qt6 environment for Wayland - combine paths from all Qt packages
            export QT_QPA_PLATFORM=wayland
            export QT_PLUGIN_PATH="${pkgs.qt6.qtbase}/lib/qt-6/plugins:${pkgs.qt6.qtwayland}/lib/qt-6/plugins"
            export QML2_IMPORT_PATH="${pkgs.qt6.qtdeclarative}/lib/qt-6/qml:${pkgs.qt6.qt5compat}/lib/qt-6/qml"
            export QML_IMPORT_PATH="$QML2_IMPORT_PATH"
            echo "Qt6 Wayland environment configured"
          '';
        };
      }
    );
}
