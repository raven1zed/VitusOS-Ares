# Edit this configuration file to define what should be installed on
# your system. Help is available in the configuration.nix(5) man page, on
# https://search.nixos.org/options and in the NixOS manual (`nixos-help`).

# NixOS-WSL specific options are documented on the NixOS-WSL repository:
# https://github.com/nix-community/NixOS-WSL

{ config, lib, pkgs, ... }:

{
  imports = [
    # include NixOS-WSL modules
    <nixos-wsl/modules>
  ];

  wsl.enable = true;
  wsl.defaultUser = "nixos";

  # VitusOS Ares Development Environment
  environment.systemPackages = with pkgs; [
    # Build tools
    cmake
    ninja
    pkg-config
    clang
    llvmPackages.libcxxClang
    gdb
    gnumake
    git
    
    # Wayland/Graphics
    wlroots
    wayland
    wayland.dev
    wayland-protocols
    wayland-scanner
    weston
    libxkbcommon
    libxkbcommon.dev
    libdrm
    libdrm.dev
    libinput
    libinput.dev
    pixman
    seatd
    mesa
    mesa.dev
    vulkan-loader
    vulkan-tools
    vulkan-headers
    vulkan-validation-layers
    
    # Qt6
    qt6.qtbase
    qt6.qtwayland
    qt6.qtdeclarative
    qt6.qtsvg
    qt6.qt5compat
    qt6.wrapQtAppsHook
    
    # Other dependencies
    glib
    glib.dev
    fontconfig
    freetype
    pam
    libxml2
    libxml2.dev
    pcre2
    pcre2.dev
  ];

  # This value determines the NixOS release from which the default
  # settings for stateful data, like file locations and database versions
  # on your system were taken. It's perfectly fine and recommended to leave
  # this value at the release version of the first install of this system.
  # Before changing this value read the documentation for this option
  # (e.g. man configuration.nix or on https://nixos.org/nixos/options.html).
  system.stateVersion = "25.05"; # Did you read the comment?
}
