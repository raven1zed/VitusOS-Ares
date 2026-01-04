# 🚀 Accelerated Development: Running on NixOS

To fast-track development and allow the AI Assistant (Antigravity) to **compile and run code directly**, you should connect your VS Code environment to the NixOS VM.

This eliminates the "write on Windows -> sync -> compile on Linux" friction.

## 1. Prerequisites
- **VS Code Extension**: Install **Remote - SSH** (ms-vscode-remote.remote-ssh).
- **NixOS VM**: Ensure your VM is running and has an SSH server enabled.
  - Forward port 2222 (or similar) to the VM's port 22.

## 2. Connect to NixOS
1.  Open VS Code Command Palette (`Ctrl+Shift+P`).
2.  Type: `Remote-SSH: Connect to Host...`.
3.  Enter your VM's SSH address, e.g., `user@localhost -p 2222`.
4.  Enter password/key if prompted.

## 3. Open the Project
1.  Once connected (green indicator in bottom left), go to **File > Open Folder**.
2.  Select the directory where `VitusOS Ares` is located inside the VM.

## ⚡ Benefits
*   **Direct Execution**: Antigravity can run `cmake`, `ninja`, and `./osf-shell` directly in the terminal panel.
*   **Real-time Errors**: Compiler errors appear immediately in the IDE.
*   **Wayland Integration**: You can run the compositor and interact with it (if using a nested backend or forwarding).

## ⚠️ Notes for AI Assistant
Once you are in this mode, simply ask the AI:
> *"Build and run the shell."*

Antigravity will detect the Linux environment and execute the build commands automatically.
