#include "TB303Synth.h"
#include "TB303Voice.h"

TB303Synth::TB303Synth()
{
}

void TB303Synth::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    for (int i = 0; i < getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<TB303Voice*>(getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock);
        }
    }
}

void TB303Synth::updateParameters(juce::AudioProcessorValueTreeState& parameters)
{
    for (int i = 0; i < getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<TB303Voice*>(getVoice(i)))
        {
            voice->updateParameters(parameters);
        }
    }
}