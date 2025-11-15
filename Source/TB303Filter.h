#pragma once

#include <JuceHeader.h>
#include <cmath>

class TB303Filter
{
public:
    TB303Filter()
    {
        reset();
    }

    void setSampleRate(double newSampleRate)
    {
        sampleRate = newSampleRate;
    }

    void setCutoff(float cutoffHz)
    {
        cutoff = juce::jlimit(20.0f, 20000.0f, cutoffHz);
        updateCoefficients();
    }

    void setResonance(float res)
    {
        // TB-303 resonance range (0-1), mapped to feedback amount
        resonance = juce::jlimit(0.0f, 1.0f, res);
        // Scale resonance to get self-oscillation at maximum
        feedback = resonance * 4.0f;
    }

    void setEnvMod(float envModAmount)
    {
        envMod = envModAmount;
    }

    void reset()
    {
        stage1 = stage2 = stage3 = 0.0f;
        lastOutput = 0.0f;
    }

    float processSample(float input, float envelopeValue)
    {
        // Modulate cutoff with envelope
        float modCutoff = cutoff * std::pow(2.0f, envMod * envelopeValue * 4.0f);
        modCutoff = juce::jlimit(20.0f, 20000.0f, modCutoff);

        // Calculate filter coefficient
        float f = 2.0f * std::sin(juce::MathConstants<float>::pi * modCutoff / sampleRate);
        f = juce::jlimit(0.0f, 1.0f, f);

        // Apply feedback (resonance)
        float inputWithFeedback = input - feedback * lastOutput;

        // Three cascaded one-pole lowpass filters (18dB/octave)
        stage1 += f * (inputWithFeedback - stage1);
        stage2 += f * (stage1 - stage2);
        stage3 += f * (stage2 - stage3);

        lastOutput = stage3;

        // Soft clipping to prevent resonance explosion
        lastOutput = std::tanh(lastOutput);

        return lastOutput;
    }

private:
    void updateCoefficients()
    {
        // Coefficients are calculated per-sample for envelope modulation
    }

    double sampleRate = 44100.0;
    float cutoff = 1000.0f;
    float resonance = 0.5f;
    float feedback = 2.0f;
    float envMod = 0.5f;

    // Filter state
    float stage1 = 0.0f;
    float stage2 = 0.0f;
    float stage3 = 0.0f;
    float lastOutput = 0.0f;
};
