/*
  ==============================================================================

    CircleofFifths.h
    Created: 21 Sep 2024 4:45:52pm
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CircleButton.h"

//==============================================================================
/*
*/


class CircleofFifths : public juce::Component
{
public:
    CircleofFifths();
    ~CircleofFifths() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    std::vector<CircleButton> notesVec;
    void circleUpdate(int root);




private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CircleofFifths)
};
