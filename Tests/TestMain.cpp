#include <JuceHeader.h>

class TB303TestRunner : public juce::UnitTestRunner
{
public:
    TB303TestRunner()
    {
        // Set up test runner
    }
    
    void logMessage (const juce::String& message) override
    {
        std::cout << message << std::endl;
    }
};

int main(int argc, char* argv[])
{
    juce::initialiseJuce_GUI();
    
    TB303TestRunner runner;
    runner.runAllTests();
    
    int numFailures = runner.getNumResults();
    std::cout << "Tests completed. Failures: " << numFailures << std::endl;
    
    juce::shutdownJuce_GUI();
    
    return numFailures > 0 ? 1 : 0;
}