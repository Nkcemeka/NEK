/*
  ==============================================================================

    Screen.cpp 
    Created: 20 Aug 2024 11:11:07am
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#include "Screen.h"

//==============================================================================
Screen::Screen(NEKAudioProcessor& p) : processor(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // Chord Name Label or Display
    addAndMakeVisible(chordName);
    chordName.setText(processor.ch, juce::dontSendNotification);
    chordName.setJustificationType(juce::Justification::centred);
    chordName.setFont(juce::Font(30.0f));
    chordName.setColour(juce::Label::textColourId, juce::Colours::black);

    // Start Timer
    startTimerHz(60);
}

Screen::~Screen()
{
}

void Screen::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colour(255, 255, 170)); // Screen colour
    juce::Colour insetColour = juce::Colours::lightgrey;
    float thickness = 2.5; // inset thickness

    // Get bounds of component and create top and left
    // inset (sort of like a drop shadow effect)
    auto bounds = getLocalBounds();
    g.setColour(insetColour);
    g.drawLine((float)bounds.getX(), (float)bounds.getY(), 
        (float)bounds.getRight(), (float)bounds.getY(), thickness);
    g.drawLine((float)bounds.getX(), (float)bounds.getY(),
        (float)bounds.getX(), (float)bounds.getBottom(), thickness);
}

void Screen::timerCallback()
{
    // Is there a better way to get the chordName instead of
    // through processor???? Think about it later...
    chordName.setText(processor.ch, juce::dontSendNotification);
};

void Screen::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    auto bounds = getLocalBounds();
    chordName.setBounds(bounds);

}

