# TB-303 Emulator VST3

A virtual analog emulation of the classic Roland TB-303 synthesizer, built with JUCE framework.

## Features

- Classic TB-303 sawtooth oscillator
- Authentic 24dB/octave lowpass filter with resonance
- Filter envelope modulation
- Accent control
- MIDI input support
- VST3, AU, and Standalone formats

## Controls

- **Cutoff**: Filter cutoff frequency (20Hz - 18kHz)
- **Resonance**: Filter resonance amount
- **Env Mod**: Filter envelope modulation depth
- **Decay**: Envelope decay time
- **Accent**: Accent intensity
- **Volume**: Output volume

## Building

### Prerequisites

#### All Platforms
- CMake 3.15 or higher
- C++17 compatible compiler
- JUCE framework (included as Git submodule)

#### Platform-Specific Requirements

**Linux:**
```bash
sudo apt-get install build-essential cmake libasound2-dev libjack-jackd2-dev pkg-config
```

**macOS:**
```bash
xcode-select --install  # Install Xcode command line tools
```

**Windows:**
- Visual Studio 2019 or later with C++ workload
- Windows 10 SDK

### Quick Build (Recommended)

Use the provided cross-platform build scripts:

**Linux/macOS:**
```bash
./build.sh          # Release build
./build.sh Debug    # Debug build
```

**Windows:**
```cmd
build.bat           # Release build
build.bat Debug     # Debug build
```

### Manual Build Instructions

```bash
# Create build directory
mkdir build
cd build

# Configure (platform-specific)
# Linux/macOS:
cmake .. -DCMAKE_BUILD_TYPE=Release

# Windows (from Developer Command Prompt):
cmake .. -G "Visual Studio 16 2019" -A x64

# Build
cmake --build . --config Release --parallel
```

### Installation

Use the provided installation scripts:

**Linux/macOS:**
```bash
./install.sh        # Install Release build
./install.sh Debug  # Install Debug build
```

**Windows (Run as Administrator):**
```cmd
install.bat         # Install Release build
install.bat Debug   # Install Debug build
```

### Plugin Locations

After building, plugins are located in:
```
build/TB303Emulator_artefacts/[Release|Debug]/
├── VST3/
│   └── TB303Emulator.vst3
├── AU/ (macOS only)
│   └── TB303Emulator.component
└── Standalone/
    └── TB303Emulator[.exe|.app]
```

### Manual Installation Paths

**VST3:**
- Windows: `C:\Program Files\Common Files\VST3\`
- macOS: `~/Library/Audio/Plug-Ins/VST3/`
- Linux: `~/.vst3/` or `/usr/lib/vst3/`

**AU (macOS only):**
- macOS: `~/Library/Audio/Plug-Ins/Components/`

## Usage

1. Load the plugin in your DAW
2. Connect a MIDI track to send notes to the plugin
3. Adjust the filter cutoff and resonance to taste
4. Use envelope modulation for classic TB-303 sweep effects
5. Apply accent to emphasize certain notes

## Development

### Creating Releases

**Automated (Recommended):**
1. Go to GitHub Actions → "Create Release" workflow
2. Click "Run workflow" 
3. Select version type (patch/minor/major) and add release notes
4. The workflow will automatically build and publish the release

**Manual:**
```bash
./release.sh patch "Bug fixes and improvements"  # Patch release
./release.sh minor "New features added"          # Minor release  
./release.sh major "Breaking changes"            # Major release
git push origin main --follow-tags               # Push to trigger build
```

### CI/CD

The project includes GitHub Actions workflows that:
- Build VST3/AU plugins for Windows, macOS, and Linux on every push
- Create automatic releases with downloadable binaries when tags are pushed
- Package plugins in platform-appropriate formats (ZIP/tar.gz)

## Downloads

Pre-built binaries are available on the [Releases](../../releases) page:
- **Windows**: `.zip` file with VST3 and Standalone
- **macOS**: `.zip` file with VST3, AU, and Standalone  
- **Linux**: `.tar.gz` file with VST3 and Standalone

## License

This project is for educational purposes. The TB-303 name and design are trademarks of Roland Corporation.