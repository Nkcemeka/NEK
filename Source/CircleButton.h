/*
  ==============================================================================

    CircleButton.h
    Created: 21 Sep 2024 5:29:40pm
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CircleButton : public juce::TextButton
{
public:
    CircleButton();
    ~CircleButton() override;
    void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
    juce::String getKey();
    void setKey(juce::String);
    juce::Colour color{ juce::Colours::white };
    juce::Colour textColor{ juce::Colours::black };
    juce::String key;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CircleButton)
};
