#!/bin/bash
# build.sh - Build all VitusOS-Ares components
# Builds framework → compositor → shell → native apps in correct order

set -e  # Exit on error

echo "╔════════════════════════════════════════════╗"
echo "║  VitusOS Ares - Build All Components      ║"
echo "╚════════════════════════════════════════════╝"
echo ""

# Check if we're in correct directory
if [ ! -f "flake.nix" ]; then
    echo "❌ Error: Must run from VitusOS-Ares root directory"
    exit 1
fi

# Clean previous builds if requested
if [ "$1" == "clean" ]; then
    echo "🧹 Cleaning previous builds..."
    rm -rf opensef/*/build
    rm -rf opensef/apps/*/build
    echo "✅ Clean complete"
    echo ""
fi

# Build function
build_component() {
    local component=$1
    local name=$2
    
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo "🔨 Building: $name"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    
    cd "$component"
    
    if [ ! -d "build" ]; then
        mkdir -p build
    fi
    
    cd build
    
    # Configure with CMake
    if [ ! -f "build.ninja" ]; then
        echo "⚙️  Configuring..."
        if [[ "$component" == *"shell"* ]]; then
            # Shell needs framework path
            cmake .. -G Ninja \
                -DCMAKE_BUILD_TYPE=Debug \
                -DCMAKE_PREFIX_PATH="$(pwd)/../../../opensef-framework/build"
        else
            cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug
        fi
    fi
    
    # Build
    echo "🔧 Compiling..."
    ninja
    
    if [ $? -eq 0 ]; then
        echo "✅ $name built successfully"
    else
        echo "❌ $name build failed"
        exit 1
    fi
        
    echo ""
    cd ../..
}

# Build in dependency order
echo "Building components in correct order..."
echo ""

# 1. Framework (required by everything)
build_component "opensef/opensef-framework" "openSEF Framework"

# 2. Compositor (displays windows)
build_component "opensef/opensef-compositor" "openSEF Compositor"

# 3. GNUstep (required by native apps)
build_component "opensef/opensef-gnustep" "GNUstep C++ Fork"

# 4. Shell (Qt Quick UI)
build_component "opensef/opensef-shell-qt" "openSEF Shell"

# 5. Native Apps
build_component "opensef/apps/osf-filer-native" "Filer (File Manager)"

echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "✅ All components built successfully!"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""
echo "🎯 Next steps:"
echo "   Run: ./scripts/run.sh"
echo ""
