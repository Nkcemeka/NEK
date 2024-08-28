/*
  ==============================================================================

    CustomMidiKeyboardComponent.cpp
    Created: 20 Aug 2024 4:36:05pm
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomMidiKeyboardComponent.h"

//==============================================================================
CustomMidiKeyboardComponent::CustomMidiKeyboardComponent(juce::MidiKeyboardState& state, Orientation orientation) :
    juce::MidiKeyboardComponent(state, orientation) {

}

juce::String CustomMidiKeyboardComponent::getWhiteNoteText(int midiNumber) {
    return "";
}
