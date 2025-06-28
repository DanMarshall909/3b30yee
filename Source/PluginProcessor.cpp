#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "TB303Voice.h"

TB303EmulatorAudioProcessor::TB303EmulatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
       parameters(*this, nullptr, "TB303Parameters", 
       {
           std::make_unique<juce::AudioParameterFloat>("cutoff", "Cutoff", 0.0f, 1.0f, 0.5f),
           std::make_unique<juce::AudioParameterFloat>("resonance", "Resonance", 0.0f, 1.0f, 0.3f),
           std::make_unique<juce::AudioParameterFloat>("envmod", "Env Mod", 0.0f, 1.0f, 0.5f),
           std::make_unique<juce::AudioParameterFloat>("decay", "Decay", 0.0f, 1.0f, 0.5f),
           std::make_unique<juce::AudioParameterFloat>("accent", "Accent", 0.0f, 1.0f, 0.0f),
           std::make_unique<juce::AudioParameterFloat>("volume", "Volume", 0.0f, 1.0f, 0.7f)
       })
{
    synth.addSound(new TB303Sound());
    
    for (int i = 0; i < 16; ++i)
        synth.addVoice(new TB303Voice());
}

TB303EmulatorAudioProcessor::~TB303EmulatorAudioProcessor()
{
}

const juce::String TB303EmulatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TB303EmulatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TB303EmulatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TB303EmulatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TB303EmulatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TB303EmulatorAudioProcessor::getNumPrograms()
{
    return 1;
}

int TB303EmulatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TB303EmulatorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TB303EmulatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void TB303EmulatorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void TB303EmulatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    synth.prepareToPlay(sampleRate, samplesPerBlock);
}

void TB303EmulatorAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TB303EmulatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TB303EmulatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    synth.updateParameters(parameters);
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

bool TB303EmulatorAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* TB303EmulatorAudioProcessor::createEditor()
{
    return new TB303EmulatorAudioProcessorEditor (*this);
}

void TB303EmulatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void TB303EmulatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TB303EmulatorAudioProcessor();
}