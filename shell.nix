# shell.nix - Simple development shell for VitusOS Ares
# Usage: nix-shell (no flakes needed!)

{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "opensef-dev";
  
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
    systemd  # for libudev
    libxcb
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
    glm
  ];

  shellHook = ''
    echo ""
    echo "=== VitusOS Ares Dev Shell ==="
    echo ""
    echo "wlroots: $(pkg-config --modversion wlroots 2>/dev/null || echo 'NOT FOUND')"
    echo "cairo:   $(pkg-config --modversion cairo 2>/dev/null || echo 'NOT FOUND')"
    echo ""
    
    export CC=clang
    export CXX=clang++
    echo "Using Clang compiler (required for Objective-C support)"
    echo ""
    echo "Build: cmake -B build -G Ninja && cmake --build build"
    echo ""
  '';
}
