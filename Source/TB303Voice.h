#pragma once

#include <JuceHeader.h>
#include "TB303Filter.h"
#include "TB303Envelope.h"

class TB303Voice : public juce::SynthesiserVoice
{
public:
    TB303Voice()
    {
        ampEnvelope.setDecay(0.3f);
        filterEnvelope.setDecay(0.3f);
    }

    bool canPlaySound(juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
    }

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int) override
    {
        // Handle slide/portamento
        if (isVoiceActive() && slideEnabled)
        {
            // Slide from current note to new note
            targetFrequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
            isSliding = true;
        }
        else
        {
            // Start new note immediately
            currentFrequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
            targetFrequency = currentFrequency;
            isSliding = false;
            phase = 0.0;
        }

        level = velocity;
        currentMidiNote = midiNoteNumber;

        // Trigger envelopes (accent if velocity > 0.7)
        bool isAccented = velocity > 0.7f;
        ampEnvelope.noteOn(isAccented);
        filterEnvelope.noteOn(isAccented);
    }

    void stopNote(float, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            ampEnvelope.noteOff();
            filterEnvelope.noteOff();
        }
        else
        {
            clearCurrentNote();
            ampEnvelope.reset();
            filterEnvelope.reset();
        }
    }

    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}

    void prepareToPlay(double sampleRate, int)
    {
        currentSampleRate = sampleRate;
        filter.setSampleRate(sampleRate);
        ampEnvelope.setSampleRate(sampleRate);
        filterEnvelope.setSampleRate(sampleRate);
    }

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        if (!isVoiceActive())
            return;

        while (--numSamples >= 0)
        {
            // Update slide/portamento
            if (isSliding)
            {
                float slideSpeed = slideTime > 0.0f ? 1.0f / (slideTime * currentSampleRate) : 1.0f;
                currentFrequency += (targetFrequency - currentFrequency) * slideSpeed;

                if (std::abs(currentFrequency - targetFrequency) < 0.1f)
                {
                    currentFrequency = targetFrequency;
                    isSliding = false;
                }
            }

            // Generate oscillator sample
            float sample = generateOscillator();

            // Get envelope values
            float ampEnv = ampEnvelope.getNextSample();
            float filterEnv = filterEnvelope.getNextSample();

            // Apply filter with envelope modulation
            sample = filter.processSample(sample, filterEnv);

            // Apply amplitude envelope
            sample *= ampEnv * level;

            // Add to output buffer
            for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            {
                outputBuffer.addSample(channel, startSample, sample);
            }

            ++startSample;

            // Check if note should end
            if (ampEnv < 0.001f && !isSliding)
            {
                clearCurrentNote();
                break;
            }
        }
    }

    // Parameter setters
    void setWaveform(int waveformType)
    {
        waveform = waveformType; // 0 = sawtooth, 1 = square
    }

    void setFilterCutoff(float cutoff)
    {
        filter.setCutoff(cutoff);
    }

    void setFilterResonance(float resonance)
    {
        filter.setResonance(resonance);
    }

    void setEnvMod(float envMod)
    {
        filter.setEnvMod(envMod);
    }

    void setDecay(float decay)
    {
        ampEnvelope.setDecay(decay);
        filterEnvelope.setDecay(decay * 0.7f); // Filter envelope is typically shorter
    }

    void setAccent(float accent)
    {
        ampEnvelope.setAccent(accent);
        filterEnvelope.setAccent(accent);
    }

    void setSlideTime(float time)
    {
        slideTime = time;
    }

    void setSlideEnabled(bool enabled)
    {
        slideEnabled = enabled;
    }

private:
    float generateOscillator()
    {
        float sample = 0.0f;

        // Update phase
        phase += currentFrequency / currentSampleRate;
        if (phase >= 1.0)
            phase -= 1.0;

        if (waveform == 0) // Sawtooth
        {
            sample = (phase * 2.0f) - 1.0f;
        }
        else // Square
        {
            sample = (phase < 0.5f) ? 1.0f : -1.0f;
        }

        return sample;
    }

    // Oscillator
    double phase = 0.0;
    double currentFrequency = 440.0;
    double targetFrequency = 440.0;
    int waveform = 0; // 0 = saw, 1 = square
    double currentSampleRate = 44100.0;

    // Envelopes
    TB303Envelope ampEnvelope;
    TB303Envelope filterEnvelope;

    // Filter
    TB303Filter filter;

    // Slide/Portamento
    bool slideEnabled = false;
    bool isSliding = false;
    float slideTime = 0.1f; // seconds

    // Voice state
    float level = 0.0f;
    int currentMidiNote = -1;
};
