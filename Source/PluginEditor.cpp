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
    // Set LookAndFeel
    juce::LookAndFeel::setDefaultLookAndFeel(&lnf);

    // Start Timer
    startTimerHz(60);

    // Setup stuff for keyboard Component
    keyboardComponent.setLowestVisibleKey(21);
    keyboardComponent.setScrollButtonWidth(1);
    keyboardComponent.setKeyWidth(15);

    // Make our components visible
    addAndMakeVisible(keyboardComponent);
    addAndMakeVisible(chordScreen);
    addAndMakeVisible(flatToggle);
    addAndMakeVisible(sharpToggle);
    addAndMakeVisible(visualizer);
    addAndMakeVisible(grandStave);
    addAndMakeVisible(keyMenu);
    addAndMakeVisible(keyMenuLabel);

    // add Key options
    keyMenu.setColour(juce::ComboBox::outlineColourId, juce::Colours::transparentBlack);
    updateKeyMenu();
    keyMenu.onChange = [this] {keyMenuChanged(); };

    // Init keyMenuLabel
    keyMenuLabel.setFont(juce::Font(20.0f, juce::Font::bold));
    keyMenuLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    keyMenuLabel.setText("Key: ", juce::NotificationType::dontSendNotification);
    
    // Init Buttons setup 
    handleButton();

    // Flat and Sharp button settings
    flatToggle.setClickingTogglesState(true);
    flatToggle.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    flatToggle.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    flatToggle.setColour(juce::ComboBox::outlineColourId, juce::Colours::transparentBlack);
    flatToggle.setButtonText("FLAT");
    flatToggle.onClick = [this] {audioProcessor.flats = !audioProcessor.flats;
    handleButton();
    updateKeyMenu();
        };

    sharpToggle.setClickingTogglesState(true);
    sharpToggle.setColour(juce::ComboBox::outlineColourId, juce::Colours::transparentBlack);
    sharpToggle.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    sharpToggle.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    sharpToggle.setButtonText("SHARP");
    sharpToggle.onClick = [this] {audioProcessor.flats = !audioProcessor.flats;
    handleButton();
    updateKeyMenu();
        };

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (900, 300);
}

NEKAudioProcessorEditor::~NEKAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void NEKAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colour::Colour(255, 255, 255));
    logo = juce::ImageCache::getFromMemory(BinaryData::g12_png, BinaryData::g12_pngSize);
    int x = 15;
    int y = 10; // originally 15
    g.drawImageWithin(logo, x, y, 37, 38, juce::RectanglePlacement::centred);
    
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

    // Setup our visualizer
    visualizer.setBounds(50, y - 46, 200, 200);

    // Setup the Grand Stave
    grandStave.setBounds(650, y-40, 200, 180);

    // Setup key Menu popup
    keyMenu.setBounds(700, y - 40, 60, 20);

    // Setup key Menu Label
    keyMenuLabel.setBounds(645, y - 40, 40, 20);
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
    grandStave.toggleState = state; // update the Grand Stave's state

    if (state) {
        // This means flats should be on and
        // sharps should be off

        // Change colour of flatToggle to On
        flatToggle.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
        flatToggle.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);

        // Change colour of sharpToggle to off
        sharpToggle.setColour(juce::TextButton::buttonColourId, juce::Colour(139, 0, 0));
        sharpToggle.setColour(juce::TextButton::buttonOnColourId, juce::Colour(139, 0, 0));

        // Change notes in visualizer to flats
        std::vector<int> temp{ 2, 3, 4, 5, 6 };
        for (auto elem : temp) {
            // eqn here is this: posCircle = (5 x posCircle) % 12
            // Hence for audioProcessor, we pass (5 * elem) % 12
            // RHS is the pos in 12TET pitch class system
            visualizer.notesVec[elem].key = audioProcessor.noteMapF[(int)(5 * elem) % 12];;
        }
    }
    else {
        // Change colour of sharpToggle to On
        sharpToggle.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
        sharpToggle.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);

        // Change colour of flatToggle to off
        flatToggle.setColour(juce::TextButton::buttonColourId, juce::Colour(139, 0, 0));
        flatToggle.setColour(juce::TextButton::buttonOnColourId, juce::Colour(139, 0, 0));

        // Change notes in visualizer to sharps
        std::vector<int> temp{ 2, 3, 4, 5, 6 };
        for (auto elem : temp) {
            // eqn here is this: posCircle = (5 x posCircle) % 12
            // Hence for audioProcessor, we pass (5 * elem) % 12
            // RHS is the pos in 12TET pitch class system
            visualizer.notesVec[elem].key = audioProcessor.noteMap[(int)(5 * elem) % 12];;
        }
    }
    grandStave.changeBounds(); // pass changes to Stave as well for proper engraving
}

void NEKAudioProcessorEditor::timerCallback()
{
    // Call circleUpdate
    visualizer.circleUpdate(audioProcessor.rootNote);
}

void NEKAudioProcessorEditor::keyMenuChanged()
{
    grandStave.setKey(keyMenu.getText());
    grandStave.repaint();
}


void NEKAudioProcessorEditor::updateKeyMenu()
{
    // clear current items
    keyMenu.clear();

    if (audioProcessor.flats) {
        keyMenu.addItem("F", 1);
        keyMenu.addItem(juce::CharPointer_UTF8("B\u266d"), 2);
        keyMenu.addItem(juce::CharPointer_UTF8("E\u266d"), 3);
        keyMenu.addItem(juce::CharPointer_UTF8("A\u266d"), 4);
        keyMenu.addItem(juce::CharPointer_UTF8("D\u266d"), 5);
        keyMenu.addItem(juce::CharPointer_UTF8("G\u266d"), 6);
        keyMenu.addItem(juce::CharPointer_UTF8("C\u266d"), 7);
    }
    else {

        keyMenu.addItem("C", 1);
        keyMenu.addItem("G", 2);
        keyMenu.addItem("D", 3);
        keyMenu.addItem("A", 4);
        keyMenu.addItem("E", 5);
        keyMenu.addItem("B", 6);
        keyMenu.addItem("F#", 7);
        keyMenu.addItem("C#", 8);
    }


    keyMenu.setSelectedId(1);
}