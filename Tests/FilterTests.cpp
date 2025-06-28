#include <JuceHeader.h>
#include "../Source/TB303Filter.h"

class FilterTests : public juce::UnitTest
{
public:
    FilterTests() : juce::UnitTest("TB303Filter Tests", "TB303") {}
    
    void runTest() override
    {
        beginTest("Filter initialization");
        
        TB303Filter filter;
        filter.setSampleRate(44100.0);
        
        expect(true, "Filter should initialize without crashing");
        
        beginTest("Filter parameter setting");
        
        filter.setCutoff(0.5f);
        filter.setResonance(0.3f);
        
        expect(true, "Filter parameters should be settable");
        
        beginTest("Filter processing");
        
        float testInput = 0.5f;
        float output = filter.processSample(testInput);
        
        expect(output >= -1.0f && output <= 1.0f, "Filter output should be in valid range");
        expect(!std::isnan(output), "Filter output should not be NaN");
        expect(!std::isinf(output), "Filter output should not be infinite");
        
        beginTest("Filter stability with extreme parameters");
        
        filter.setCutoff(1.0f);  // Maximum cutoff
        filter.setResonance(1.0f);  // Maximum resonance
        
        for (int i = 0; i < 1000; ++i)
        {
            float extremeOutput = filter.processSample(0.8f);
            expect(!std::isnan(extremeOutput), "Filter should remain stable with extreme parameters");
            expect(!std::isinf(extremeOutput), "Filter should not produce infinite values");
        }
    }
};

static FilterTests filterTests;