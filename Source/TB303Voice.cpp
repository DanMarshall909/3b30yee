#include "TB303Voice.h"
#include "TB303Synth.h"

TB303Voice::TB303Voice()
{
    adsrParams.attack = 0.01f;
    adsrParams.decay = 0.5f;
    adsrParams.sustain = 0.0f;
    adsrParams.release = 0.1f;
    
    envFilterParams.attack = 0.01f;
    envFilterParams.decay = 0.3f;
    envFilterParams.sustain = 0.0f;
    envFilterParams.release = 0.1f;
}

bool TB303Voice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<TB303Sound*>(sound) != nullptr;
}

void TB303Voice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    angleDelta = frequency * 2.0 * juce::MathConstants<double>::pi / sampleRate;
    
    adsr.noteOn();
    envFilter.noteOn();
    
    // Apply accent if present
    float accentMultiplier = 1.0f + accent * 2.0f;
    adsrParams.decay = decay * accentMultiplier;
    envFilterParams.decay = decay * 0.5f * accentMultiplier;
    
    adsr.setParameters(adsrParams);
    envFilter.setParameters(envFilterParams);
}

void TB303Voice::stopNote(float velocity, bool allowTailOff)
{
    adsr.noteOff();
    envFilter.noteOff();
    
    if (!allowTailOff || !adsr.isActive())
        clearCurrentNote();
}

void TB303Voice::pitchWheelMoved(int newPitchWheelValue)
{
}

void TB303Voice::controllerMoved(int controllerNumber, int newControllerValue)
{
}

void TB303Voice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (!adsr.isActive())
        return;
        
    while (--numSamples >= 0)
    {
        // Generate sawtooth wave
        float sample = (float)(2.0 * currentAngle / juce::MathConstants<double>::pi - 1.0);
        
        currentAngle += angleDelta;
        if (currentAngle >= juce::MathConstants<double>::twoPi)
            currentAngle -= juce::MathConstants<double>::twoPi;
        
        // Apply amplitude envelope
        sample *= adsr.getNextSample();
        
        // Apply filter with envelope modulation
        float filterEnv = envFilter.getNextSample();
        float modCutoff = cutoff + (envMod * filterEnv);
        modCutoff = juce::jlimit(0.0f, 1.0f, modCutoff);
        
        filter.setCutoff(modCutoff);
        filter.setResonance(resonance);
        sample = filter.processSample(sample);
        
        // Apply volume
        sample *= volume;
        
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
            outputBuffer.addSample(channel, startSample, sample);
            
        ++startSample;
    }
}

void TB303Voice::prepareToPlay(double newSampleRate, int samplesPerBlock)
{
    sampleRate = newSampleRate;
    adsr.setSampleRate(sampleRate);
    envFilter.setSampleRate(sampleRate);
    filter.setSampleRate(sampleRate);
}

void TB303Voice::updateParameters(juce::AudioProcessorValueTreeState& parameters)
{
    cutoff = *parameters.getRawParameterValue("cutoff");
    resonance = *parameters.getRawParameterValue("resonance");
    envMod = *parameters.getRawParameterValue("envmod");
    decay = *parameters.getRawParameterValue("decay");
    accent = *parameters.getRawParameterValue("accent");
    volume = *parameters.getRawParameterValue("volume");
    
    adsrParams.decay = decay;
    envFilterParams.decay = decay * 0.5f;
    
    adsr.setParameters(adsrParams);
    envFilter.setParameters(envFilterParams);
}