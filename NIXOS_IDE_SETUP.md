# 🐧 Installing "Antigravity" (IDE) on NixOS

You asked how to install "Antigravity" (this AI Assistant/IDE) directly on NixOS to speed up development.

## Option 1: The "Headless" Install (Recommended for VMs) 🚀
**You likely already did this!**

When you use **VS Code Remote - SSH** to connect to your NixOS VM:
1.  VS Code installs its **Server** component inside NixOS.
2.  It installs the **Antigravity Extension** inside NixOS.
3.  The AI logic, compilation, and terminal commands **run 100% on Linux**.

**This is the fastest method.** It gives you the Windows GUI (smooth) with the Linux Backend (fast compiling), and the Agents run directly on the Linux CPU.

---

## Option 2: The Native Install (If you want the GUI on Linux) 🖥️
If you are running NixOS on bare metal (not a VM) or have a graphical interface (GNOME/KDE) in your VM and want the IDE window there:

### 1. Edit your NixOS Configuration
Add `vscode` to your system packages in `/etc/nixos/configuration.nix`:

```nix
{ pkgs, ... }: {
  environment.systemPackages = with pkgs; [
    git
    cmake
    ninja
    gcc
    # The IDE
    vscode
  ];
}
```

### 2. Apply Changes
Run:
```bash
sudo nixos-rebuild switch
```

### 3. Install the Extension
1.  Open the newly installed VS Code on Linux.
2.  Sign in to your account (GitHub/Microsoft) to sync settings.
3.  Ensure the AI Assistant extension is installed.

---

## 💡 Which one allows "Antigravity" to code fastest?
**Both are equal.** 
Since Remote-SSH runs the *backend* on Linux, the AI has the exact same access to `cmake`, `bash`, and files as the Native install.

### Recommendation
*   **Use Remote-SSH** if you are on Windows hosting a Linux VM.
*   **Use Native** only if you are logging directly into a Linux Desktop.
