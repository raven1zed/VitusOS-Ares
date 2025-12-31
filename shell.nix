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
    gcc
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
  ];

  shellHook = ''
    echo ""
    echo "=== VitusOS Ares Dev Shell ==="
    echo ""
    echo "wlroots: $(pkg-config --modversion wlroots 2>/dev/null || echo 'NOT FOUND')"
    echo "cairo:   $(pkg-config --modversion cairo 2>/dev/null || echo 'NOT FOUND')"
    echo ""
    echo "Build: cd opensef && mkdir -p build && cd build && cmake .. -G Ninja && ninja"
    echo ""
  '';
}
