#!/bin/bash

# Cross-platform build script for TB-303 Emulator
# Usage: ./build.sh [Debug|Release]

set -e

BUILD_TYPE=${1:-Release}
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"

echo "Building TB-303 Emulator ($BUILD_TYPE)..."
echo "Platform: $(uname -s)"

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Platform-specific setup
case "$(uname -s)" in
    Linux*)
        echo "Setting up Linux build..."
        # Check for required packages
        if ! pkg-config --exists alsa; then
            echo "Installing required packages..."
            sudo apt-get update
            sudo apt-get install -y build-essential cmake libasound2-dev libjack-jackd2-dev pkg-config libfreetype6-dev libx11-dev libxext-dev libxrandr-dev libxinerama-dev libxcursor-dev
        fi
        CMAKE_ARGS=""
        ;;
    Darwin*)
        echo "Setting up macOS build..."
        # Check for Xcode command line tools
        if ! xcode-select -p &> /dev/null; then
            echo "Please install Xcode command line tools:"
            echo "xcode-select --install"
            exit 1
        fi
        CMAKE_ARGS=""
        ;;
    CYGWIN*|MINGW*|MSYS*)
        echo "Setting up Windows build..."
        CMAKE_ARGS="-G \"Visual Studio 16 2019\" -A x64"
        ;;
    *)
        echo "Unknown platform: $(uname -s)"
        exit 1
        ;;
esac

# Configure
echo "Configuring CMake..."
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE $CMAKE_ARGS

# Build
echo "Building..."
cmake --build . --config $BUILD_TYPE --parallel $(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo ""
echo "Build completed successfully!"
echo ""

# Show output locations
case "$(uname -s)" in
    Linux*)
        echo "VST3 plugin: $BUILD_DIR/TB303Emulator_artefacts/$BUILD_TYPE/VST3/TB303Emulator.vst3"
        echo "Standalone: $BUILD_DIR/TB303Emulator_artefacts/$BUILD_TYPE/Standalone/TB303Emulator"
        echo ""
        echo "To install VST3, copy to: ~/.vst3/ or /usr/lib/vst3/"
        ;;
    Darwin*)
        echo "VST3 plugin: $BUILD_DIR/TB303Emulator_artefacts/$BUILD_TYPE/VST3/TB303Emulator.vst3"
        echo "AU plugin: $BUILD_DIR/TB303Emulator_artefacts/$BUILD_TYPE/AU/TB303Emulator.component"
        echo "Standalone: $BUILD_DIR/TB303Emulator_artefacts/$BUILD_TYPE/Standalone/TB303Emulator.app"
        echo ""
        echo "To install:"
        echo "  VST3: cp -r *.vst3 ~/Library/Audio/Plug-Ins/VST3/"
        echo "  AU: cp -r *.component ~/Library/Audio/Plug-Ins/Components/"
        ;;
    CYGWIN*|MINGW*|MSYS*)
        echo "VST3 plugin: $BUILD_DIR/TB303Emulator_artefacts/$BUILD_TYPE/VST3/TB303Emulator.vst3"
        echo "Standalone: $BUILD_DIR/TB303Emulator_artefacts/$BUILD_TYPE/Standalone/TB303Emulator.exe"
        echo ""
        echo "To install VST3, copy to: C:\\Program Files\\Common Files\\VST3\\"
        ;;
esac