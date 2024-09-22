/*
  ==============================================================================

    CircleButton.cpp
    Created: 21 Sep 2024 5:29:40pm
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CircleButton.h"

//==============================================================================

CircleButton::CircleButton()
{
}

CircleButton::~CircleButton()
{
}


juce::String CircleButton::getKey()
{
    return key;
}

void CircleButton::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    auto bounds = getLocalBounds();

    g.setColour(color);
    g.fillEllipse(bounds.toFloat().reduced(5));

    // set font and draw text
    g.setColour(textColor);
    g.setFont(13.0f);
    g.drawText(key, bounds, juce::Justification::centred, true);
}

void CircleButton::setKey(juce::String note)
{
    key = note;
}
