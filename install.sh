#!/bin/bash

# Installation script for TB-303 Emulator
# Usage: ./install.sh [Debug|Release]

set -e

BUILD_TYPE=${1:-Release}
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"

echo "Installing TB-303 Emulator ($BUILD_TYPE)..."

# Check if build exists
if [ ! -d "$BUILD_DIR/TB303Emulator_artefacts/$BUILD_TYPE" ]; then
    echo "Build not found. Please run ./build.sh first."
    exit 1
fi

# Platform-specific installation
case "$(uname -s)" in
    Linux*)
        echo "Installing on Linux..."
        VST3_SRC="$BUILD_DIR/TB303Emulator_artefacts/$BUILD_TYPE/VST3/TB303Emulator.vst3"
        
        # Try user directory first, then system directory
        if [ -w "$HOME/.vst3" ] || mkdir -p "$HOME/.vst3" 2>/dev/null; then
            VST3_DEST="$HOME/.vst3/"
            echo "Installing VST3 to user directory: $VST3_DEST"
        elif [ -w "/usr/lib/vst3" ] || sudo mkdir -p "/usr/lib/vst3" 2>/dev/null; then
            VST3_DEST="/usr/lib/vst3/"
            echo "Installing VST3 to system directory: $VST3_DEST"
            SUDO_CMD="sudo"
        else
            echo "Cannot create VST3 directory. Please install manually."
            exit 1
        fi
        
        $SUDO_CMD cp -r "$VST3_SRC" "$VST3_DEST"
        echo "VST3 installed successfully!"
        ;;
        
    Darwin*)
        echo "Installing on macOS..."
        VST3_SRC="$BUILD_DIR/TB303Emulator_artefacts/$BUILD_TYPE/VST3/TB303Emulator.vst3"
        AU_SRC="$BUILD_DIR/TB303Emulator_artefacts/$BUILD_TYPE/AU/TB303Emulator.component"
        
        mkdir -p "$HOME/Library/Audio/Plug-Ins/VST3"
        mkdir -p "$HOME/Library/Audio/Plug-Ins/Components"
        
        if [ -d "$VST3_SRC" ]; then
            cp -r "$VST3_SRC" "$HOME/Library/Audio/Plug-Ins/VST3/"
            echo "VST3 installed successfully!"
        fi
        
        if [ -d "$AU_SRC" ]; then
            cp -r "$AU_SRC" "$HOME/Library/Audio/Plug-Ins/Components/"
            echo "AU installed successfully!"
        fi
        ;;
        
    CYGWIN*|MINGW*|MSYS*)
        echo "Please run install.bat on Windows"
        exit 1
        ;;
        
    *)
        echo "Unknown platform: $(uname -s)"
        exit 1
        ;;
esac

echo ""
echo "Installation completed!"
echo "Restart your DAW to detect the new plugin."