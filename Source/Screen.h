/*
  ==============================================================================

    Screen.h
    Created: 20 Aug 2024 11:11:07am
    Author:  Chukwuemeka L. Nkama
    Description: Contains declarations necessary for building the screen 
                 to display chord names

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class Screen  : public juce::Component, public juce::Timer
{
public:
    Screen(NEKAudioProcessor& processor);
    ~Screen() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    juce::Label chordName; // Chord Message Box
    NEKAudioProcessor& processor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Screen)
};
