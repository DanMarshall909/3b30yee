#include "PluginProcessor.h"
#include "PluginEditor.h"

TB303AudioProcessorEditor::TB303AudioProcessorEditor(TB303AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(700, 450);

    // Title
    titleLabel.setText("TB-303 Clone", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(32.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(titleLabel);

    // Waveform selector
    waveformLabel.setText("Waveform", juce::dontSendNotification);
    waveformLabel.setJustificationType(juce::Justification::centred);
    waveformLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(waveformLabel);

    waveformSelector.addItem("Sawtooth", 1);
    waveformSelector.addItem("Square", 2);
    waveformSelector.setSelectedId(1);
    addAndMakeVisible(waveformSelector);
    waveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getValueTreeState(), "waveform", waveformSelector);

    // Setup sliders
    setupSlider(cutoffSlider, cutoffLabel, "Cutoff");
    setupSlider(resonanceSlider, resonanceLabel, "Resonance");
    setupSlider(envModSlider, envModLabel, "Env Mod");
    setupSlider(decaySlider, decayLabel, "Decay");
    setupSlider(accentSlider, accentLabel, "Accent");
    setupSlider(slideSlider, slideLabel, "Slide");
    setupSlider(volumeSlider, volumeLabel, "Volume");

    // Create attachments
    cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "cutoff", cutoffSlider);
    resonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "resonance", resonanceSlider);
    envModAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "envmod", envModSlider);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "decay", decaySlider);
    accentAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "accent", accentSlider);
    slideAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "slide", slideSlider);
    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), "volume", volumeSlider);
}

TB303AudioProcessorEditor::~TB303AudioProcessorEditor()
{
}

void TB303AudioProcessorEditor::setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& labelText)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff808080));
    slider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffd0d0d0));
    slider.setColour(juce::Slider::trackColourId, juce::Colour(0xff404040));
    addAndMakeVisible(slider);

    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(label);
}

void TB303AudioProcessorEditor::paint(juce::Graphics& g)
{
    // Background gradient (silver/grey TB-303 style)
    g.fillAll(juce::Colour(0xff2a2a2a));

    auto bounds = getLocalBounds();

    // Header section
    auto headerArea = bounds.removeFromTop(80);
    g.setGradientFill(juce::ColourGradient(
        juce::Colour(0xff505050), 0, 0,
        juce::Colour(0xff303030), 0, headerArea.getHeight(), false));
    g.fillRect(headerArea);

    // Main panel
    g.setGradientFill(juce::ColourGradient(
        juce::Colour(0xff404040), 0, headerArea.getBottom(),
        juce::Colour(0xff202020), 0, bounds.getBottom(), false));
    g.fillRect(bounds);

    // Draw panel borders
    g.setColour(juce::Colour(0xff606060));
    g.drawRect(getLocalBounds(), 2);

    // Oscillator section
    g.setColour(juce::Colour(0xff505050));
    g.fillRoundedRectangle(20, 100, 160, 120, 5);
    g.setColour(juce::Colour(0xff707070));
    g.drawRoundedRectangle(20, 100, 160, 120, 5, 2);

    // Filter section
    g.setColour(juce::Colour(0xff505050));
    g.fillRoundedRectangle(200, 100, 280, 120, 5);
    g.setColour(juce::Colour(0xff707070));
    g.drawRoundedRectangle(200, 100, 280, 120, 5, 2);

    // Envelope section
    g.setColour(juce::Colour(0xff505050));
    g.fillRoundedRectangle(20, 240, 280, 120, 5);
    g.setColour(juce::Colour(0xff707070));
    g.drawRoundedRectangle(20, 240, 280, 120, 5, 2);

    // Output section
    g.setColour(juce::Colour(0xff505050));
    g.fillRoundedRectangle(320, 240, 160, 120, 5);
    g.setColour(juce::Colour(0xff707070));
    g.drawRoundedRectangle(320, 240, 160, 120, 5, 2);

    // Section labels
    g.setColour(juce::Colours::lightgrey);
    g.setFont(juce::Font(14.0f, juce::Font::bold));
    g.drawText("OSCILLATOR", 20, 100, 160, 20, juce::Justification::centred);
    g.drawText("FILTER", 200, 100, 280, 20, juce::Justification::centred);
    g.drawText("ENVELOPE", 20, 240, 280, 20, juce::Justification::centred);
    g.drawText("OUTPUT", 320, 240, 160, 20, juce::Justification::centred);
}

void TB303AudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    // Title
    titleLabel.setBounds(bounds.removeFromTop(80).reduced(10));

    // Oscillator section (left top)
    waveformLabel.setBounds(30, 125, 140, 20);
    waveformSelector.setBounds(40, 150, 120, 30);

    // Filter section (middle top)
    cutoffLabel.setBounds(210, 125, 80, 20);
    cutoffSlider.setBounds(205, 145, 90, 90);

    resonanceLabel.setBounds(300, 125, 80, 20);
    resonanceSlider.setBounds(295, 145, 90, 90);

    envModLabel.setBounds(390, 125, 80, 20);
    envModSlider.setBounds(385, 145, 90, 90);

    // Envelope section (bottom left)
    decayLabel.setBounds(30, 265, 80, 20);
    decaySlider.setBounds(25, 285, 90, 90);

    accentLabel.setBounds(120, 265, 80, 20);
    accentSlider.setBounds(115, 285, 90, 90);

    slideLabel.setBounds(210, 265, 80, 20);
    slideSlider.setBounds(205, 285, 90, 90);

    // Output section (bottom right)
    volumeLabel.setBounds(330, 265, 140, 20);
    volumeSlider.setBounds(345, 285, 110, 90);
}

