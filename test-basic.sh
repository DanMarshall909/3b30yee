#!/bin/bash

# Basic test script that doesn't require CMake
# Just checks if the source files compile individually

echo "Testing TB-303 Emulator source files..."

# Check if g++ is available
if ! command -v g++ &> /dev/null; then
    echo "g++ not available, skipping local tests"
    exit 0
fi

echo "Testing individual file compilation..."

# Test if each source file can be parsed (syntax check)
echo "Checking TB303Filter.cpp..."
g++ -std=c++17 -fsyntax-only Source/TB303Filter.cpp -I. 2>/dev/null && echo "✅ TB303Filter.cpp syntax OK" || echo "❌ TB303Filter.cpp syntax error"

echo "Checking TB303Voice.cpp..."
g++ -std=c++17 -fsyntax-only Source/TB303Voice.cpp -I. 2>/dev/null && echo "✅ TB303Voice.cpp syntax OK" || echo "❌ TB303Voice.cpp syntax error"

echo "Checking TB303Synth.cpp..."
g++ -std=c++17 -fsyntax-only Source/TB303Synth.cpp -I. 2>/dev/null && echo "✅ TB303Synth.cpp syntax OK" || echo "❌ TB303Synth.cpp syntax error"

echo ""
echo "Note: Full compilation requires JUCE framework and CMake"
echo "This is just a basic syntax check."