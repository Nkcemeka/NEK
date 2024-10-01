/*
  ==============================================================================

    NoteHead.h
    Created: 23 Sep 2024 5:43:39pm
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//==============================================================================
/*
*/

class NoteHead  : public juce::Component
{
public:
    NoteHead();
    ~NoteHead() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    bool noteOn{ 0 };
    juce::String acc{" "}; // accidental before note

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteHead)
};
