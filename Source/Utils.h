/*
  ==============================================================================

    Utils.h
    Created: 10 Oct 2024 5:40:25pm
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#include <JuceHeader.h>
#pragma once

// Big thanks to Matthjis Hollemans for this
template<typename T>
inline static void castParameter(juce::AudioProcessorValueTreeState& apvts,
    const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);
}