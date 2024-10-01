/*
  ==============================================================================

    NoteHead.cpp
    Created: 23 Sep 2024 5:43:39pm
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#include <JuceHeader.h>
#include "NoteHead.h"

//==============================================================================
NoteHead::NoteHead()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}

NoteHead::~NoteHead()
{
}

void NoteHead::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    // get Bounds of noteHead
    auto bounds = getLocalBounds().toFloat();
    auto x = bounds.getX();
    auto y = bounds.getY();
    auto width = bounds.getWidth();
    auto height = bounds.getHeight();

    // Draw ellipse for noteHead and the corresponding accidental
    if (noteOn) {
        g.setColour(juce::Colours::black);
        g.setFont(21.0f);
        g.drawText(acc, juce::Rectangle<float>{x+2, y-6, width/4, height}, juce::Justification::centred);
        g.fillEllipse(juce::Rectangle<float> {x + width/4, y + height/4, width/4, height/4});
    }
}

void NoteHead::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}