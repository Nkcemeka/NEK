/*
  ==============================================================================

    CustomMidiKeyboardComponent.h
    Created: 20 Aug 2024 4:36:05pm
    Author:  kevin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CustomMidiKeyboardComponent  : public juce::MidiKeyboardComponent
{
public:
    CustomMidiKeyboardComponent(juce::MidiKeyboardState& state, Orientation orientation);
    juce::String getWhiteNoteText(int) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomMidiKeyboardComponent)
};
