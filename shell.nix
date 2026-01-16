{ pkgs ? import (fetchTarball "https://github.com/NixOS/nixpkgs/archive/nixos-unstable.tar.gz") {} }:

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    cmake
    ninja
    pkg-config
    wayland-scanner
    whitesur-icon-theme
    imagemagick
    clang
    gdb
    gnumake
    git
    sysprof # Add sysprof to nativeBuildInputs for pkg-config
  ];
  
  buildInputs = with pkgs; [
    wlroots
    wayland
    wayland-protocols
    libxkbcommon
    libdrm
    libinput
    libgbm
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
    pam
    sysprof
    libdisplay-info
    libxml2
    pcre2
    util-linux
    # Qt6 components
    qt6.qtbase
    qt6.qtwayland
    qt6.qtdeclarative
    qt6.qtsvg
    qt6.qt5compat     # For Qt5Compat.GraphicalEffects
    qt6.qtshadertools # REQUIRED for QML Shaders in Qt 6
    qt6.qtimageformats # For SVG/WebP support
    
    xorg.xclock
    weston
  ];

  shellHook = ''
    # Ensure all dev outputs are in PKG_CONFIG_PATH
    export PKG_CONFIG_PATH="${pkgs.wlroots}/lib/pkgconfig:${pkgs.wayland.dev}/lib/pkgconfig:${pkgs.wayland-protocols}/share/pkgconfig:${pkgs.cairo.dev}/lib/pkgconfig:${pkgs.pango.dev}/lib/pkgconfig:${pkgs.libxkbcommon.dev}/lib/pkgconfig:${pkgs.mesa}/lib/pkgconfig:${pkgs.pam}/lib/pkgconfig:${pkgs.sysprof.dev}/lib/pkgconfig:${pkgs.sysprof.lib}/lib/pkgconfig:${pkgs.libxml2.dev}/lib/pkgconfig:${pkgs.pcre2.dev}/lib/pkgconfig:${pkgs.util-linux.dev}/lib/pkgconfig:${pkgs.glib.dev}/lib/pkgconfig:${pkgs.harfbuzz.dev}/lib/pkgconfig:${pkgs.fontconfig.dev}/lib/pkgconfig:$PKG_CONFIG_PATH"
    export CC=clang
    export CXX=clang++
    export CXXFLAGS="-D_GLIBCXX_USE_CXX11_ABI=1"
    
    # Qt6 environment for Wayland shell
    export QT_QPA_PLATFORM=wayland
    export XDG_DATA_DIRS="${pkgs.whitesur-icon-theme}/share:$XDG_DATA_DIRS"
    export QT_PLUGIN_PATH="${pkgs.qt6.qtbase}/lib/qt-6/plugins:${pkgs.qt6.qtwayland}/lib/qt-6/plugins:${pkgs.qt6.qtsvg}/lib/qt-6/plugins:${pkgs.qt6.qt5compat}/lib/qt-6/plugins:${pkgs.qt6.qtimageformats}/lib/qt-6/plugins"
    export QML2_IMPORT_PATH="${pkgs.qt6.qtdeclarative}/lib/qt-6/qml:${pkgs.qt6.qt5compat}/lib/qt-6/qml:${pkgs.qt6.qtshadertools}/lib/qt-6/qml"
    export QML_IMPORT_PATH="$QML2_IMPORT_PATH"
    echo "Qt6 Wayland environment configured with Unstable Packages"
  '';
}
