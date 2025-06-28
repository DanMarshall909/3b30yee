#include <JuceHeader.h>
#include "../Source/PluginProcessor.h"

class ProcessorTests : public juce::UnitTest
{
public:
    ProcessorTests() : juce::UnitTest("TB303Processor Tests", "TB303") {}
    
    void runTest() override
    {
        beginTest("Processor initialization");
        
        TB303EmulatorAudioProcessor processor;
        
        expect(processor.getName() == "TB-303 Emulator", "Processor should have correct name");
        expect(processor.acceptsMidi(), "Processor should accept MIDI");
        expect(!processor.producesMidi(), "Processor should not produce MIDI");
        expect(!processor.isMidiEffect(), "Processor should not be a MIDI effect");
        
        beginTest("Processor preparation");
        
        processor.prepareToPlay(44100.0, 512);
        expect(true, "Processor should prepare without crashing");
        
        beginTest("MIDI and audio processing");
        
        juce::AudioBuffer<float> buffer(2, 512);
        juce::MidiBuffer midiBuffer;
        
        // Add a MIDI note on message
        juce::MidiMessage noteOn = juce::MidiMessage::noteOn(1, 60, 0.8f);
        midiBuffer.addEvent(noteOn, 0);
        
        // Add a MIDI note off message later
        juce::MidiMessage noteOff = juce::MidiMessage::noteOff(1, 60, 0.5f);
        midiBuffer.addEvent(noteOff, 256);
        
        buffer.clear();
        processor.processBlock(buffer, midiBuffer);
        
        expect(true, "Processor should handle MIDI and audio without crashing");
        
        beginTest("Parameter access");
        
        auto* cutoffParam = processor.parameters.getRawParameterValue("cutoff");
        auto* resonanceParam = processor.parameters.getRawParameterValue("resonance");
        auto* volumeParam = processor.parameters.getRawParameterValue("volume");
        
        expect(cutoffParam != nullptr, "Cutoff parameter should exist");
        expect(resonanceParam != nullptr, "Resonance parameter should exist");
        expect(volumeParam != nullptr, "Volume parameter should exist");
        
        if (cutoffParam)
        {
            expect(*cutoffParam >= 0.0f && *cutoffParam <= 1.0f, "Cutoff parameter should be in valid range");
        }
        
        beginTest("State save/restore");
        
        juce::MemoryBlock stateData;
        processor.getStateInformation(stateData);
        
        expect(stateData.getSize() > 0, "State data should not be empty");
        
        processor.setStateInformation(stateData.getData(), (int)stateData.getSize());
        expect(true, "State should be restorable without crashing");
        
        processor.releaseResources();
    }
};

static ProcessorTests processorTests;