#pragma once

#include <JuceHeader.h>
#include "TB303Voice.h"

class TB303Sound : public juce::SynthesiserSound
{
public:
    bool appliesToNote (int midiNoteNumber) override { return true; }
    bool appliesToChannel (int midiChannel) override { return true; }
};

class TB303Synth : public juce::Synthesiser
{
public:
    TB303Synth();
    
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void updateParameters(juce::AudioProcessorValueTreeState& parameters);
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TB303Synth)
};