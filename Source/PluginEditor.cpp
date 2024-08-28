/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NEKAudioProcessorEditor::NEKAudioProcessorEditor (NEKAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), keyboardComponent(audioProcessor.keyboardState,
        juce::MidiKeyboardComponent::horizontalKeyboard), chordScreen(p)
{
    // Setup stuff for keyboard Component
    keyboardComponent.setLowestVisibleKey(21);
    keyboardComponent.setScrollButtonWidth(1);
    keyboardComponent.setKeyWidth(15);

    // Make our components visible
    addAndMakeVisible(keyboardComponent);
    addAndMakeVisible(chordScreen);
    addAndMakeVisible(flatToggle);
    addAndMakeVisible(sharpToggle);

    // Iniit Buttons setup 
    handleButton();

    // Flat and Sharp button settings
    flatToggle.setClickingTogglesState(true);
    flatToggle.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    flatToggle.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    flatToggle.setButtonText("FLAT");
    flatToggle.onClick = [this] {audioProcessor.flats = !audioProcessor.flats;
    handleButton(); };

    sharpToggle.setClickingTogglesState(true);
    sharpToggle.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    sharpToggle.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    sharpToggle.setButtonText("SHARP");
    sharpToggle.onClick = [this] {audioProcessor.flats = !audioProcessor.flats;
    handleButton();
        };

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (900, 300);
}

NEKAudioProcessorEditor::~NEKAudioProcessorEditor()
{
}

//==============================================================================
void NEKAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colour::Colour(255, 255, 255));
    logo = juce::ImageCache::getFromMemory(BinaryData::g12_png, BinaryData::g12_pngSize);
    g.drawImageWithin(logo, 15, 15, 37, 38, juce::RectanglePlacement::stretchToFit);
    
}

void NEKAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto area = getLocalBounds();
    keyboardComponent.setBounds(area.removeFromBottom(80).reduced(8));

    // Calculate the position to center the object
    int x = (area.getWidth() - 300) / 2;
    int y = (area.getHeight() - 100) / 2;

    // Create a rectangle for the object's bounds
    // vars such as new_x_flat or new_x_sharp are arbitrary
    int new_x_flat = x + 80;
    int new_y_flat = y + 110;
    int new_x_sharp = x + 170;
    int new_y_sharp = y + 110;
    chordScreen.setBounds(x, y, 300, 100);
    flatToggle.setBounds(new_x_flat, new_y_flat, 55, 25);
    sharpToggle.setBounds(new_x_sharp, new_y_sharp, 55, 25);
}

void NEKAudioProcessorEditor::setAccidental(bool state)
{
    // Set Accidental to sharp or flat
    audioProcessor.flats = state;
}

void NEKAudioProcessorEditor::handleButton() {
    // Get the current state of the flat param in
    // audioProcessor
    bool state = audioProcessor.flats;

    if (state) {
        // This means flats should be on and
        // sharps should be off

        // Change colour of flatToggle to On
        flatToggle.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
        flatToggle.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);

        // Change colour of sharpToggle to off
        sharpToggle.setColour(juce::TextButton::buttonColourId, juce::Colour(139, 0, 0));
        sharpToggle.setColour(juce::TextButton::buttonOnColourId, juce::Colour(139, 0, 0));
    }
    else {
        // Change colour of sharpToggle to On
        sharpToggle.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
        sharpToggle.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);

        // Change colour of flatToggle to off
        flatToggle.setColour(juce::TextButton::buttonColourId, juce::Colour(139, 0, 0));
        flatToggle.setColour(juce::TextButton::buttonOnColourId, juce::Colour(139, 0, 0));
    }
}