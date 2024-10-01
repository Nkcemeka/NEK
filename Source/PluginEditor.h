/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LookAndFeel.h"
#include "PluginProcessor.h"
#include "Screen.h"
#include "CustomMidiKeyboardComponent.h"
#include "CircleofFifths.h"
#include "Stave.h"

//==============================================================================
/**
*/


class NEKAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer
{
public:

    NEKAudioProcessorEditor (NEKAudioProcessor&);
    ~NEKAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void keyMenuChanged();
    void updateKeyMenu();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NEKAudioProcessor& audioProcessor;
    CustomMidiKeyboardComponent keyboardComponent;
    Screen chordScreen; // Chord Screen
    juce::TextButton flatToggle; 
    juce::TextButton sharpToggle;
    void setAccidental(bool state);
    void handleButton();
    juce::Image logo;
    CircleofFifths visualizer;
    LookAndFeel lnf;
    Stave grandStave{ audioProcessor.keyboardBuffer};
    juce::ComboBox keyMenu;
    juce::Label keyMenuLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NEKAudioProcessorEditor)
};
