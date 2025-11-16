#include "PluginProcessor.h"
#include "PluginEditor.h"

TB303AudioProcessor::TB303AudioProcessor()
    : AudioProcessor(BusesProperties()
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    // Add voices to synthesizer
    for (int i = 0; i < 8; ++i)
        synth.addVoice(new TB303Voice());

    synth.addSound(new TB303Sound());
}

TB303AudioProcessor::~TB303AudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout TB303AudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Waveform (0 = Saw, 1 = Square)
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "waveform",
        "Waveform",
        juce::StringArray{"Sawtooth", "Square"},
        0));

    // Filter Cutoff (20 Hz to 20 kHz)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "cutoff",
        "Cutoff",
        juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f),
        1000.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value, 1) + " Hz"; }));

    // Filter Resonance (0 to 1)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "resonance",
        "Resonance",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.5f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(int(value * 100)) + " %"; }));

    // Envelope Modulation (0 to 1)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "envmod",
        "Env Mod",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.5f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(int(value * 100)) + " %"; }));

    // Decay (0.01 to 2 seconds)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "decay",
        "Decay",
        juce::NormalisableRange<float>(0.01f, 2.0f, 0.01f, 0.5f),
        0.3f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(value, 2) + " s"; }));

    // Accent (0 to 1)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "accent",
        "Accent",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.5f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(int(value * 100)) + " %"; }));

    // Slide Time (0 to 0.5 seconds)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "slide",
        "Slide",
        juce::NormalisableRange<float>(0.0f, 0.5f, 0.01f),
        0.1f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(int(value * 1000)) + " ms"; }));

    // Master Volume
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "volume",
        "Volume",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.7f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) { return juce::String(int(value * 100)) + " %"; }));

    return {params.begin(), params.end()};
}

const juce::String TB303AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TB303AudioProcessor::acceptsMidi() const
{
    return true;
}

bool TB303AudioProcessor::producesMidi() const
{
    return false;
}

bool TB303AudioProcessor::isMidiEffect() const
{
    return false;
}

double TB303AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TB303AudioProcessor::getNumPrograms()
{
    return 1;
}

int TB303AudioProcessor::getCurrentProgram()
{
    return 0;
}

void TB303AudioProcessor::setCurrentProgram(int)
{
}

const juce::String TB303AudioProcessor::getProgramName(int)
{
    return {};
}

void TB303AudioProcessor::changeProgramName(int, const juce::String&)
{
}

void TB303AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<TB303Voice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock);
        }
    }
}

void TB303AudioProcessor::releaseResources()
{
}

bool TB303AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void TB303AudioProcessor::updateVoiceParameters()
{
    int waveform = apvts.getRawParameterValue("waveform")->load();
    float cutoff = apvts.getRawParameterValue("cutoff")->load();
    float resonance = apvts.getRawParameterValue("resonance")->load();
    float envmod = apvts.getRawParameterValue("envmod")->load();
    float decay = apvts.getRawParameterValue("decay")->load();
    float accent = apvts.getRawParameterValue("accent")->load();
    float slide = apvts.getRawParameterValue("slide")->load();

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<TB303Voice*>(synth.getVoice(i)))
        {
            voice->setWaveform(waveform);
            voice->setFilterCutoff(cutoff);
            voice->setFilterResonance(resonance);
            voice->setEnvMod(envmod);
            voice->setDecay(decay);
            voice->setAccent(accent);
            voice->setSlideTime(slide);
            voice->setSlideEnabled(slide > 0.001f);
        }
    }
}

void TB303AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    buffer.clear();

    // Update voice parameters
    updateVoiceParameters();

    // Render synth
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // Apply master volume
    float volume = apvts.getRawParameterValue("volume")->load();
    buffer.applyGain(volume);
}

bool TB303AudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* TB303AudioProcessor::createEditor()
{
    return new TB303AudioProcessorEditor(*this);
}

void TB303AudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void TB303AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TB303AudioProcessor();
}
