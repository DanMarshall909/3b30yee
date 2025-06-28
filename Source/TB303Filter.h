#pragma once

#include <JuceHeader.h>

class TB303Filter
{
public:
    TB303Filter();
    
    void setSampleRate(double sampleRate);
    void setCutoff(float cutoff);
    void setResonance(float resonance);
    float processSample(float input);
    void reset();
    
private:
    void updateCoefficients();
    
    double sampleRate = 44100.0;
    float cutoffFreq = 1000.0f;
    float resonanceAmount = 0.3f;
    
    // 24dB/octave lowpass filter (4-pole)
    float input1 = 0.0f, input2 = 0.0f, input3 = 0.0f, input4 = 0.0f;
    float output1 = 0.0f, output2 = 0.0f, output3 = 0.0f, output4 = 0.0f;
    
    float frequency = 0.0f;
    float resonance = 0.0f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TB303Filter)
};