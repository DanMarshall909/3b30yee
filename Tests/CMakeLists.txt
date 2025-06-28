# Test configuration for TB-303 Emulator

# Create test executable with source files
add_executable(TB303Tests
    FilterTests.cpp
    VoiceTests.cpp
    ProcessorTests.cpp
    TestMain.cpp
    ../Source/TB303Filter.cpp
    ../Source/TB303Voice.cpp
    ../Source/TB303Synth.cpp
    ../Source/PluginProcessor.cpp)

# Link with JUCE modules
target_link_libraries(TB303Tests
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_dsp
        juce::juce_gui_basics
        juce::juce_recommended_config_flags)

# Include directories
target_include_directories(TB303Tests PRIVATE
    ../Source)

# Compile definitions for tests
target_compile_definitions(TB303Tests
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_UNIT_TESTS=1
        $<$<PLATFORM_ID:Linux>:JUCE_JACK=1>
        $<$<PLATFORM_ID:Linux>:JUCE_ALSA=1>
        $<$<PLATFORM_ID:Windows>:JUCE_WASAPI=1>
        $<$<PLATFORM_ID:Windows>:JUCE_DIRECTSOUND=1>
        $<$<PLATFORM_ID:Darwin>:JUCE_COREAUDIO=1>)

# Platform-specific linking for tests
if(WIN32)
    target_link_libraries(TB303Tests PRIVATE winmm)
elseif(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(ALSA REQUIRED alsa)
    pkg_check_modules(FREETYPE REQUIRED freetype2)
    pkg_check_modules(X11 REQUIRED x11 xext xrandr xinerama xcursor)
    
    target_link_libraries(TB303Tests PRIVATE 
        ${ALSA_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        ${X11_LIBRARIES}
        Threads::Threads 
        dl)
elseif(APPLE)
    target_link_libraries(TB303Tests PRIVATE
        "-framework CoreAudio"
        "-framework CoreMIDI" 
        "-framework AudioUnit"
        "-framework AudioToolbox"
        "-framework Accelerate"
        "-framework Cocoa")
endif()

# Enable testing
enable_testing()
add_test(NAME TB303UnitTests COMMAND TB303Tests)