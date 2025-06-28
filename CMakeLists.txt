cmake_minimum_required(VERSION 3.15)

project(TB303Emulator VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific settings
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "Minimum OS X deployment version")
    # Enable universal binaries on Apple Silicon
    if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "arm64")
        set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Build architectures for Mac OS X" FORCE)
    endif()
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add JUCE as a subdirectory
add_subdirectory(JUCE)

# Platform-specific formats
if(APPLE)
    set(PLUGIN_FORMATS AU VST3 Standalone)
elseif(WIN32)
    set(PLUGIN_FORMATS VST3 Standalone)
else()
    set(PLUGIN_FORMATS VST3 Standalone)
endif()

# Define our plugin target
juce_add_plugin(TB303Emulator
    COMPANY_NAME "TB303Studio"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE TB3S
    PLUGIN_CODE TB03
    FORMATS ${PLUGIN_FORMATS}
    PRODUCT_NAME "TB-303 Emulator")

# Source files
target_sources(TB303Emulator
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/TB303Synth.cpp
        Source/TB303Voice.cpp
        Source/TB303Filter.cpp)

# Link JUCE modules
target_link_libraries(TB303Emulator
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

# Platform-specific compile definitions
target_compile_definitions(TB303Emulator
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        $<$<PLATFORM_ID:Linux>:JUCE_JACK=1>
        $<$<PLATFORM_ID:Linux>:JUCE_ALSA=1>
        $<$<PLATFORM_ID:Windows>:JUCE_WASAPI=1>
        $<$<PLATFORM_ID:Windows>:JUCE_DIRECTSOUND=1>
        $<$<PLATFORM_ID:Darwin>:JUCE_COREAUDIO=1>)

# Platform-specific linking
if(WIN32)
    target_link_libraries(TB303Emulator PRIVATE winmm)
elseif(UNIX AND NOT APPLE)
    # Linux dependencies
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(ALSA REQUIRED alsa)
    target_link_libraries(TB303Emulator PRIVATE ${ALSA_LIBRARIES})
    target_include_directories(TB303Emulator PRIVATE ${ALSA_INCLUDE_DIRS})
    
    # Optional JACK support
    pkg_check_modules(JACK jack)
    if(JACK_FOUND)
        target_link_libraries(TB303Emulator PRIVATE ${JACK_LIBRARIES})
        target_include_directories(TB303Emulator PRIVATE ${JACK_INCLUDE_DIRS})
    endif()
    
    # Threading and other Linux deps
    find_package(Threads REQUIRED)
    target_link_libraries(TB303Emulator PRIVATE Threads::Threads dl)
elseif(APPLE)
    # macOS frameworks
    target_link_libraries(TB303Emulator PRIVATE
        "-framework CoreAudio"
        "-framework CoreMIDI" 
        "-framework AudioUnit"
        "-framework AudioToolbox"
        "-framework Accelerate")
endif()