/*
  ==============================================================================

    LookAndFeel.h
    Created: 22 Sep 2024 6:02:58am
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class LookAndFeel : public juce::LookAndFeel_V4 {
public:
    LookAndFeel();
    void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
        bool isSeparator, bool isActive, bool isHighlighted, bool isTicked,
        bool hasSubMenu, const juce::String& text, const juce::String& shortcutKeyText,
        const juce::Drawable* icon, const juce::Colour* textColourToUse) override;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LookAndFeel);
};
