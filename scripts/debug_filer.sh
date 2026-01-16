#!/bin/bash
# Debug script for Filer launch issues

# 1. Set Library Paths based on build structure
export LD_LIBRARY_PATH=$(pwd)/opensef/opensef-framework/build:$(pwd)/opensef/opensef-core/build:$(pwd)/opensef/opensef-base/build:$(pwd)/opensef/opensef-gnustep/build:/usr/lib/wsl/lib:$LD_LIBRARY_PATH

# Force D3D12 Driver for WSL2 Hardware Acceleration
# Dynamically find the path to d3d12_dri.so (preferring the version matching the system mesa) using fast lookup
D3D12_DRIVER_DIR=$(ls -d /nix/store/*mesa-25.0.7*/lib/dri 2>/dev/null | head -n 1)
if [ -n "$D3D12_DRIVER_DIR" ]; then
    export LIBGL_DRIVERS_PATH=$D3D12_DRIVER_DIR
    export MESA_LOADER_DRIVER_OVERRIDE=d3d12
    export GALLIUM_DRIVER=d3d12
    echo "GPU Acceleration Enabled: Using d3d12 driver from $D3D12_DRIVER_DIR"
else
    echo "WARNING: d3d12_dri.so not found! Falling back to software rendering."
fi

# 2. Enable Critical Qt/Wayland Logging
export QT_LOGGING_RULES="*.debug=true;qt.scenegraph.general=true"
export QSG_INFO=1
export WAYLAND_DEBUG=1

# 3. Explicitly request OpenGL (Hardware) backend
export QSG_RHI_BACKEND=opengl

# 4. Launch Filer and capture EVERYTHING
echo "Starting Filer Debug..."
./opensef/apps/osf-filer-native/build/osf-filer-native > filer_debug.log 2>&1
