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
  ];

  shellHook = ''
    export PKG_CONFIG_PATH="${pkgs.wlroots}/lib/pkgconfig:${pkgs.wayland.dev}/lib/pkgconfig:${pkgs.wayland-protocols}/share/pkgconfig:${pkgs.cairo.dev}/lib/pkgconfig:${pkgs.pango.dev}/lib/pkgconfig:${pkgs.libxkbcommon.dev}/lib/pkgconfig:${pkgs.mesa}/lib/pkgconfig:$PKG_CONFIG_PATH"
    export CC=clang
    export CXX=clang++
  '';
}
