#pragma once

#include <JuceHeader.h>
#include "TB303Filter.h"

class TB303Voice : public juce::SynthesiserVoice
{
public:
    TB303Voice();
    
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void updateParameters(juce::AudioProcessorValueTreeState& parameters);
    
private:
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    
    double currentAngle = 0.0;
    double angleDelta = 0.0;
    double frequency = 0.0;
    double sampleRate = 44100.0;
    
    TB303Filter filter;
    
    float cutoff = 0.5f;
    float resonance = 0.3f;
    float envMod = 0.5f;
    float decay = 0.5f;
    float accent = 0.0f;
    float volume = 0.7f;
    
    juce::ADSR envFilter;
    juce::ADSR::Parameters envFilterParams;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TB303Voice)
};