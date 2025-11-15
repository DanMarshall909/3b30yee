# TB-303 Clone VST

A fully functional Roland TB-303 Bass Line clone implemented as a VST3 plugin using JUCE framework.

## Features

### Authentic TB-303 Sound Engine
- **Oscillator**: Sawtooth and Square waveforms
- **Filter**: 18dB/octave (3-pole) resonant low-pass filter with authentic TB-303 character
- **Envelope**: Exponential decay envelope with accent control
- **Slide**: Portamento/glide between notes for classic TB-303 basslines
- **Modulation**: Filter envelope modulation for dynamic timbral changes

### Parameters

| Parameter | Range | Description |
|-----------|-------|-------------|
| **Waveform** | Sawtooth/Square | Oscillator waveform selection |
| **Cutoff** | 20 Hz - 20 kHz | Filter cutoff frequency |
| **Resonance** | 0% - 100% | Filter resonance (self-oscillates at high values) |
| **Env Mod** | 0% - 100% | Amount of envelope modulation applied to filter cutoff |
| **Decay** | 10 ms - 2 s | Envelope decay time |
| **Accent** | 0% - 100% | Accent amount (increases envelope peak and modulation) |
| **Slide** | 0 - 500 ms | Portamento/slide time between notes |
| **Volume** | 0% - 100% | Master output volume |

## Technical Details

### Architecture
- **Polyphony**: 8 voices
- **Filter**: 3-pole cascade design with feedback for resonance
- **Envelope**: Exponential decay with accent boost
- **Slide**: Frequency-based portamento with configurable time

### Sound Characteristics
- Authentic TB-303 filter modeling with self-oscillation capability
- Exponential envelope decay for characteristic "pluck" sound
- Accent adds envelope boost and increased modulation depth
- Slide creates smooth pitch transitions between notes

## Building

### Prerequisites
- CMake 3.15 or higher
- C++17 compatible compiler
  - Windows: Visual Studio 2019 or later
  - macOS: Xcode 11 or later
  - Linux: GCC 7+ or Clang 6+
- Git (for downloading JUCE)

### Build Instructions

#### All Platforms
```bash
# Clone the repository
git clone <repository-url>
cd tb303-vst-clone

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
cmake --build . --config Release
```

#### Platform-Specific Notes

**Windows:**
```bash
# Configure for Visual Studio
cmake .. -G "Visual Studio 17 2022"

# Build
cmake --build . --config Release
```
The VST3 will be in `build/TB303Clone_artefacts/Release/VST3/`

**macOS:**
```bash
# Configure for Xcode
cmake .. -G Xcode

# Build
cmake --build . --config Release
```
The VST3 will be in `build/TB303Clone_artefacts/Release/VST3/`

**Linux:**
```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install libasound2-dev libcurl4-openssl-dev libfreetype6-dev \
                     libx11-dev libxcomposite-dev libxcursor-dev libxinerama-dev \
                     libxrandr-dev libxrender-dev libgl1-mesa-dev

# Configure
cmake ..

# Build
cmake --build . --config Release
```
The VST3 will be in `build/TB303Clone_artefacts/Release/VST3/`

### Installation

#### Windows
Copy the VST3 folder to:
- `C:\Program Files\Common Files\VST3\`

#### macOS
Copy the VST3 bundle to:
- `~/Library/Audio/Plug-Ins/VST3/` (user)
- `/Library/Audio/Plug-Ins/VST3/` (system)

#### Linux
Copy the VST3 folder to:
- `~/.vst3/` (user)
- `/usr/lib/vst3/` (system)

## Usage

### Basic Operation
1. Load the plugin in your DAW
2. Select a waveform (Sawtooth for classic acid bass, Square for a different flavor)
3. Adjust the filter cutoff and resonance to shape the tone
4. Use Env Mod to add dynamic filter movement
5. Adjust Decay to control how long notes sustain
6. Use Accent on certain notes (velocity > 0.7) for emphasis
7. Enable Slide for smooth pitch transitions

### Classic TB-303 Acid Bass Sound
- **Waveform**: Sawtooth
- **Cutoff**: 500-1500 Hz
- **Resonance**: 70-90%
- **Env Mod**: 60-80%
- **Decay**: 100-300 ms
- **Accent**: 50-70%
- **Slide**: Enable on select notes (50-150 ms)

### Tips
- High resonance values will cause the filter to self-oscillate
- Accent is triggered when MIDI velocity > 0.7 (roughly velocity 90+)
- Slide works best between consecutive notes
- Combine short decay with high resonance for classic "squelch" sounds
- Experiment with square wave for different timbral character

## Project Structure

```
.
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── LICENSE                 # License file
└── Source/
    ├── PluginProcessor.h   # Main audio processor header
    ├── PluginProcessor.cpp # Main audio processor implementation
    ├── PluginEditor.h      # UI header
    ├── PluginEditor.cpp    # UI implementation
    ├── TB303Voice.h        # Synthesizer voice implementation
    ├── TB303Voice.cpp      # Voice implementation
    ├── TB303Filter.h       # 18dB/octave resonant filter
    ├── TB303Filter.cpp     # Filter implementation
    ├── TB303Envelope.h     # Envelope generator
    ├── TB303Envelope.cpp   # Envelope implementation
    ├── TB303Synth.h        # Synthesizer sound class
    └── TB303Synth.cpp      # Synth implementation
```

## Dependencies

This project uses [JUCE](https://juce.com/) framework, which is automatically downloaded during the CMake configuration via FetchContent.

## License

See LICENSE file for details.

## Credits

Inspired by the legendary Roland TB-303 Bass Line synthesizer.

## Troubleshooting

### Build Errors
- Ensure you have all prerequisites installed
- Try deleting the `build` directory and rebuilding from scratch
- Make sure your compiler supports C++17

### Plugin Not Appearing in DAW
- Verify the VST3 is copied to the correct plugin directory
- Rescan plugins in your DAW
- Check that your DAW supports VST3 format

### Audio Issues
- Check sample rate compatibility (44.1kHz or 48kHz recommended)
- Ensure buffer size is reasonable (128-512 samples)
- Reduce resonance if you hear distortion or instability

## Future Enhancements

Possible future additions:
- Pattern sequencer
- Additional filter types
- Preset management
- MIDI CC mapping
- Additional modulation sources
- Oversampling for improved filter quality

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.
