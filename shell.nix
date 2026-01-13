{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
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
    qt6.qt5compat  # For Qt5Compat.GraphicalEffects
    xorg.xclock
    weston
  ];

  shellHook = ''
    export PKG_CONFIG_PATH="${pkgs.wlroots}/lib/pkgconfig:${pkgs.wayland.dev}/lib/pkgconfig:${pkgs.wayland-protocols}/share/pkgconfig:${pkgs.cairo.dev}/lib/pkgconfig:${pkgs.pango.dev}/lib/pkgconfig:${pkgs.libxkbcommon.dev}/lib/pkgconfig:${pkgs.mesa}/lib/pkgconfig:${pkgs.pam}/lib/pkgconfig:${pkgs.sysprof.dev}/lib/pkgconfig:${pkgs.libxml2.dev}/lib/pkgconfig:${pkgs.pcre2.dev}/lib/pkgconfig:${pkgs.util-linux.dev}/lib/pkgconfig:${pkgs.glib.dev}/lib/pkgconfig:${pkgs.harfbuzz.dev}/lib/pkgconfig:$PKG_CONFIG_PATH"
    export CC=clang
    export CXX=clang++
    
    # Qt6 environment for Wayland shell
    export QT_QPA_PLATFORM=wayland
    export QT_PLUGIN_PATH="${pkgs.qt6.qtbase}/lib/qt-6/plugins:${pkgs.qt6.qtwayland}/lib/qt-6/plugins:${pkgs.qt6.qtsvg}/lib/qt-6/plugins:${pkgs.qt6.qt5compat}/lib/qt-6/plugins"
    export QML2_IMPORT_PATH="${pkgs.qt6.qtdeclarative}/lib/qt-6/qml:${pkgs.qt6.qt5compat}/lib/qt-6/qml"
    export QML_IMPORT_PATH="$QML2_IMPORT_PATH"
    echo "Qt6 Wayland environment configured"
  '';
}
