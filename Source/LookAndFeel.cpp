/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 22 Sep 2024 6:03:11am
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#include "LookAndFeel.h"

LookAndFeel::LookAndFeel()
{
    juce::Typeface::Ptr typeface = juce::Typeface::createSystemTypefaceFor(
        BinaryData::FreeSerif4aeK_ttf, BinaryData::FreeSerif4aeK_ttfSize
    );
    setDefaultSansSerifTypeface(typeface);
}

void LookAndFeel::drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
    bool isSeparator, bool isActive, bool isHighlighted, bool isTicked,
    bool hasSubMenu, const juce::String& text, const juce::String& shortcutKeyText,
    const juce::Drawable* icon, const juce::Colour* textColourToUse) {
    if (isHighlighted) g.fillAll(juce::Colour(0xff1e1e1e));

    // set text Color
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(16.0f));
    g.drawText(text, area.withTrimmedLeft(10), juce::Justification::centredLeft, true);
}
