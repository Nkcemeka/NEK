/*
  ==============================================================================

    LED.cpp
    Created: 10 Oct 2024 6:21:15am
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LED.h"

//==============================================================================
LED::LED() : trigger(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}

LED::~LED()
{
}

void LED::paint(juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    auto bounds = getLocalBounds().toFloat();
    juce::ColourGradient cgRed{ juce::Colours::red, bounds.getBottomLeft(),
        juce::Colours::lightslategrey, bounds.getTopRight(), true };
    juce::ColourGradient cgBlack{ juce::Colour(0, 0, 0), bounds.getBottomLeft(),
        juce::Colours::grey, bounds.getTopRight(), true };

    if (trigger) g.setGradientFill(cgRed);
    else g.setGradientFill(cgBlack);

    g.fillEllipse(bounds);
}

void LED::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    auto bounds = getLocalBounds();
}

void LED::setLED(bool extTrigger)
{
    trigger = extTrigger;
    repaint(); // Redraw the component since its state has changed

    // Start a timer to call the callback after 150ms if LED should be on
    startTimer(150);

}


void LED::timerCallback()
{
    trigger = false; // Turn off LED
    repaint();
    stopTimer();
}