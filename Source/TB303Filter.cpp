#include "TB303Filter.h"

TB303Filter::TB303Filter()
{
    reset();
}

void TB303Filter::setSampleRate(double newSampleRate)
{
    sampleRate = newSampleRate;
    updateCoefficients();
}

void TB303Filter::setCutoff(float cutoff)
{
    cutoffFreq = 20.0f + cutoff * cutoff * 18000.0f; // Exponential scaling
    updateCoefficients();
}

void TB303Filter::setResonance(float res)
{
    resonanceAmount = res;
    updateCoefficients();
}

void TB303Filter::updateCoefficients()
{
    frequency = 2.0f * juce::MathConstants<float>::pi * cutoffFreq / (float)sampleRate;
    frequency = juce::jlimit(0.0f, 1.0f, frequency);
    
    resonance = 1.0f - resonanceAmount;
    resonance = juce::jmax(0.01f, resonance); // Prevent instability
}

float TB303Filter::processSample(float input)
{
    // Classic TB-303 style 4-pole lowpass filter
    input1 = input1 + frequency * (input - input1 + resonance * (input1 - input4));
    input2 = input2 + frequency * (input1 - input2);
    input3 = input3 + frequency * (input2 - input3);
    input4 = input4 + frequency * (input3 - input4);
    
    // Apply slight saturation for character
    float output = input4;
    output = juce::jlimit(-1.0f, 1.0f, output * 1.2f);
    
    return output;
}

void TB303Filter::reset()
{
    input1 = input2 = input3 = input4 = 0.0f;
    output1 = output2 = output3 = output4 = 0.0f;
}