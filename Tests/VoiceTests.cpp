#include <JuceHeader.h>
#include "../Source/TB303Voice.h"
#include "../Source/TB303Synth.h"

class VoiceTests : public juce::UnitTest
{
public:
    VoiceTests() : juce::UnitTest("TB303Voice Tests", "TB303") {}
    
    void runTest() override
    {
        beginTest("Voice initialization");
        
        TB303Voice voice;
        TB303Sound sound;
        
        expect(voice.canPlaySound(&sound), "Voice should be able to play TB303Sound");
        
        beginTest("Voice note triggering");
        
        voice.prepareToPlay(44100.0, 512);
        
        // Start a note
        voice.startNote(60, 0.8f, &sound, 0);
        expect(voice.isVoiceActive(), "Voice should be active after note start");
        
        beginTest("Voice audio processing");
        
        juce::AudioBuffer<float> buffer(2, 512);
        buffer.clear();
        
        // Process some audio
        voice.renderNextBlock(buffer, 0, 512);
        
        // Check if any audio was generated
        bool hasAudio = false;
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                if (std::abs(buffer.getSample(channel, sample)) > 0.001f)
                {
                    hasAudio = true;
                    break;
                }
            }
        }
        
        expect(hasAudio, "Voice should generate audio when active");
        
        beginTest("Voice note release");
        
        voice.stopNote(0.5f, true);
        
        // Process audio to let the voice finish
        for (int i = 0; i < 10; ++i)
        {
            buffer.clear();
            voice.renderNextBlock(buffer, 0, 512);
        }
        
        expect(!voice.isVoiceActive() || voice.isPlayingButReleased(), "Voice should stop or be releasing");
    }
};

static VoiceTests voiceTests;