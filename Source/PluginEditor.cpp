#include "PluginProcessor.h"
#include "PluginEditor.h"

TB303EmulatorAudioProcessorEditor::TB303EmulatorAudioProcessorEditor (TB303EmulatorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (600, 400);
    
    // Setup sliders
    cutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    cutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(cutoffSlider);
    
    resonanceSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    resonanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(resonanceSlider);
    
    envModSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    envModSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(envModSlider);
    
    decaySlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(decaySlider);
    
    accentSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    accentSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(accentSlider);
    
    volumeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(volumeSlider);
    
    // Setup labels
    cutoffLabel.setText("Cutoff", juce::dontSendNotification);
    cutoffLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(cutoffLabel);
    
    resonanceLabel.setText("Resonance", juce::dontSendNotification);
    resonanceLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(resonanceLabel);
    
    envModLabel.setText("Env Mod", juce::dontSendNotification);
    envModLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(envModLabel);
    
    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(decayLabel);
    
    accentLabel.setText("Accent", juce::dontSendNotification);
    accentLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(accentLabel);
    
    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(volumeLabel);
    
    // Create parameter attachments
    cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "cutoff", cutoffSlider);
    resonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "resonance", resonanceSlider);
    envModAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "envmod", envModSlider);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "decay", decaySlider);
    accentAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "accent", accentSlider);
    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "volume", volumeSlider);
}

TB303EmulatorAudioProcessorEditor::~TB303EmulatorAudioProcessorEditor()
{
}

void TB303EmulatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff202020));
    
    g.setColour (juce::Colours::white);
    g.setFont (24.0f);
    g.drawFittedText ("TB-303 Emulator", getLocalBounds().removeFromTop(50), juce::Justification::centred, 1);
}

void TB303EmulatorAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(60); // Title area
    
    auto knobArea = bounds.reduced(20);
    int knobSize = 80;
    int spacing = 20;
    int totalWidth = 6 * knobSize + 5 * spacing;
    int startX = (knobArea.getWidth() - totalWidth) / 2;
    
    // Top row
    cutoffSlider.setBounds(startX, knobArea.getY(), knobSize, knobSize);
    cutoffLabel.setBounds(startX, knobArea.getY() + knobSize + 5, knobSize, 20);
    
    resonanceSlider.setBounds(startX + knobSize + spacing, knobArea.getY(), knobSize, knobSize);
    resonanceLabel.setBounds(startX + knobSize + spacing, knobArea.getY() + knobSize + 5, knobSize, 20);
    
    envModSlider.setBounds(startX + 2 * (knobSize + spacing), knobArea.getY(), knobSize, knobSize);
    envModLabel.setBounds(startX + 2 * (knobSize + spacing), knobArea.getY() + knobSize + 5, knobSize, 20);
    
    // Bottom row
    int bottomY = knobArea.getY() + 150;
    
    decaySlider.setBounds(startX + knobSize + spacing, bottomY, knobSize, knobSize);
    decayLabel.setBounds(startX + knobSize + spacing, bottomY + knobSize + 5, knobSize, 20);
    
    accentSlider.setBounds(startX + 3 * (knobSize + spacing), bottomY, knobSize, knobSize);
    accentLabel.setBounds(startX + 3 * (knobSize + spacing), bottomY + knobSize + 5, knobSize, 20);
    
    volumeSlider.setBounds(startX + 4 * (knobSize + spacing), bottomY, knobSize, knobSize);
    volumeLabel.setBounds(startX + 4 * (knobSize + spacing), bottomY + knobSize + 5, knobSize, 20);
}