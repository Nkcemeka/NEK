/*
  ==============================================================================

    Stave.cpp
    Created: 23 Sep 2024 2:50:18pm
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Stave.h"

//==============================================================================
Stave::Stave(std::vector<bool>& kb) : keyboardBuffer(kb), 
thickness(1), distStave(0), space(0), widthLedger(30), xNotes(130), 
staveNotes(56), widthNote(10)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    startTimerHz(60); // refresh Stave regularly
    
    // Add each Stave Note to the Stave
    for (int i = 0; i < staveNotes.size(); ++i)
    {
        addAndMakeVisible(staveNotes[i]);
    }
}

Stave::~Stave()
{
    stopTimer();
}

void Stave::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    auto bounds = getLocalBounds();
    
    // Set Drawing Colour
    g.setColour(juce::Colours::black);

    // Setup lines

    // Draw Treble Clef
    g.fillRect(ten);
    g.fillRect(eleven);
    g.fillRect(twelve);
    g.fillRect(thirteen);
    g.fillRect(fourteen);

    // Draw Bass Clef
    g.fillRect(four);
    g.fillRect(five);
    g.fillRect(six);
    g.fillRect(seven);
    g.fillRect(eight);

    // Draw ledger lines if necessary
    if (onef) g.fillRect(one);
    if (twof) g.fillRect(two);
    if (threef) g.fillRect(three);
    if (ninef) g.fillRect(nine);
    if (fiff) g.fillRect(fifteen);
    if (sixf) g.fillRect(sixteen);
    if (sevenf) g.fillRect(seventeen);


    // Clef Logos: This path is filled with Nasty hacks :-); I've not figured out SVGs yet
    auto bassImage = juce::ImageCache::getFromMemory(BinaryData::bassClefPNG_png,
        BinaryData::bassClefPNG_pngSize);
    auto trebleImage = juce::ImageCache::getFromMemory(BinaryData::trebleClefPNG_png,
        BinaryData::trebleClefPNG_pngSize);

    juce::Rectangle<float> targetAreaBass{ bounds.toFloat().getX() + 2, bounds.toFloat().getY() + 110,
        25 / 1.3f, 25}; // aspect ratio (1:1.3)
    juce::Rectangle<float> targetAreaTreble{ bounds.toFloat().getX(), bounds.toFloat().getY() + 33,
        22, 44}; // aspect ratio (1:2)
    g.drawImage(bassImage, targetAreaBass, juce::RectanglePlacement::stretchToFit, false);
    g.drawImage(trebleImage, targetAreaTreble, juce::RectanglePlacement::centred, false);

    // You can use drawText once you get a font
    //g.drawText(juce::CharPointer_UTF8("\xf0\x9d\x84\xa2"), targetAreaBass, juce::Justification::centredLeft);
    //g.drawText(juce::CharPointer_UTF8("\xf0\x9d\x84\x9e"), targetAreaTreble, juce::Justification::centredLeft);


    // Draw Key Singature
    // Get the bounds of the keySignature component. This should be a long rectangle
    // stretching from treble to bass clef
    auto sigBounds = juce::Rectangle<int>{bounds.getX() + 30, bounds.getY(), 60, bounds.getHeight()};
    g.setFont(20.0f);

    // draw key signature on the Stave
    if (currKey == "C") {
        // do  nothing
    }
    else if (currKey == "G") {
        juce::Rectangle<int> rect = getLine(gkey[0] - 33);
        juce::Rectangle<int> newBounds{ sigBounds.getX(), rect.getY() - 6, 10, 10 };
        g.drawText(juce::CharPointer_UTF8("\u266f"), newBounds, juce::Justification::centred);

        // Setup note or MIDI pitch on bass clef
        // This is MIDI - 24 as pitch on bass clef is two octaves below
        rect = getLine(gkey[0] - 33 - 24);
        newBounds = { sigBounds.getX(), rect.getY() - 6, 10, 10 };
        g.drawText(juce::CharPointer_UTF8("\u266f"), newBounds, juce::Justification::centred);
    }
    else if (currKey == "D") {
        int shift = 0; // for each succ. accidental
        int yshift = 4; // to account for spaces
        for (int i = 0; i < d.size(); ++i) {
            juce::Rectangle<int> rect = getLine(d[i] - 33);
            juce::Rectangle<int> newBounds;

            if (!checkSpace(d[i] - 33)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266f"), newBounds, juce::Justification::centred);

            // MIDI note for bass clef
            rect = getLine(d[i] - 33 - 24);
            if (!checkSpace(d[i] - 33 - 24)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266f"), newBounds, juce::Justification::centred);
            shift += 5;
            
        }
    }
    else if (currKey == "A") {
        int shift = 0; // for each succ. accidental
        int yshift = 4; // to account for spaces
        for (int i = 0; i < a.size(); ++i) {
            juce::Rectangle<int> rect = getLine(a[i] - 33);
            juce::Rectangle<int> newBounds;

            if (!checkSpace(a[i] - 33)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266f"), newBounds, juce::Justification::centred);

            // MIDI note for bass clef
            rect = getLine(a[i] - 33 - 24);
            if (!checkSpace(a[i] - 33 - 24)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266f"), newBounds, juce::Justification::centred);
            shift += 5;
        }
    }
    else if (currKey == "E") {
        int shift = 0; // for each succ. accidental
        int yshift = 4; // to account for spaces
        for (int i = 0; i < e.size(); ++i) {
            juce::Rectangle<int> rect = getLine(e[i] - 33);
            juce::Rectangle<int> newBounds;

            if (!checkSpace(e[i] - 33)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266f"), newBounds, juce::Justification::centred);

            // MIDI note for bass clef
            rect = getLine(e[i] - 33 - 24);
            if (!checkSpace(e[i] - 33 - 24)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266f"), newBounds, juce::Justification::centred);
            shift += 5;
        }
    }
    else if (currKey == "B") {
        int shift = 0; // for each succ. accidental
        int yshift = 4; // to account for spaces
        for (int i = 0; i < b.size(); ++i) {
            juce::Rectangle<int> rect = getLine(b[i] - 33);
            juce::Rectangle<int> newBounds;

            if (!checkSpace(b[i] - 33)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266f"), newBounds, juce::Justification::centred);

            // MIDI note for bass clef
            rect = getLine(b[i] - 33 - 24);
            if (!checkSpace(b[i] - 33 - 24)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266f"), newBounds, juce::Justification::centred);
            shift += 5;
        }
    }
    else if (currKey == "F#") {
        int shift = 0; // for each succ. accidental
        int yshift = 4; // to account for spaces
        for (int i = 0; i < fsharp.size(); ++i) {
            juce::Rectangle<int> rect = getLine(fsharp[i] - 33);
            juce::Rectangle<int> newBounds;

            if (!checkSpace(fsharp[i] - 33)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266f"), newBounds, juce::Justification::centred);

            // MIDI note for bass clef
            rect = getLine(fsharp[i] - 33 - 24);
            if (!checkSpace(fsharp[i] - 33 - 24)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266f"), newBounds, juce::Justification::centred);
            shift += 5;
        }
    }
    else if (currKey == "C#") {
        int shift = 0; // for each succ. accidental
        int yshift = 4; // to account for spaces
        for (int i = 0; i < csharp.size(); ++i) {
            juce::Rectangle<int> rect = getLine(csharp[i] - 33);
            juce::Rectangle<int> newBounds;

            if (!checkSpace(csharp[i] - 33)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266f"), newBounds, juce::Justification::centred);

            // MIDI note for bass clef
            rect = getLine(csharp[i] - 33 - 24);
            if (!checkSpace(csharp[i] - 33 - 24)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266f"), newBounds, juce::Justification::centred);
            shift += 5;
        }
    }
    else if (currKey == "F") {
        int shift = 0; // for each succ. accidental
        int yshift = 4; // to account for spaces
        for (int i = 0; i < f.size(); ++i) {
            juce::Rectangle<int> rect = getLine(f[i] - 33);
            juce::Rectangle<int> newBounds;

            if (!checkSpace(f[i] - 33)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266d"), newBounds, juce::Justification::centred);

            // MIDI note for bass clef
            rect = getLine(f[i] - 33 - 24);
            if (!checkSpace(f[i] - 33 - 24)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266d"), newBounds, juce::Justification::centred);
            shift += 5;
        }
    }
    else if (currKey == juce::CharPointer_UTF8("B\u266d")) {
        int shift = 0; // for each succ. accidental
        int yshift = 4; // to account for spaces
        for (int i = 0; i < bflat.size(); ++i) {
            juce::Rectangle<int> rect = getLine(bflat[i] - 33);
            juce::Rectangle<int> newBounds;

            if (!checkSpace(bflat[i] - 33)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266d"), newBounds, juce::Justification::centred);

            // MIDI note for bass clef
            rect = getLine(bflat[i] - 33 - 24);
            if (!checkSpace(bflat[i] - 33 - 24)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266d"), newBounds, juce::Justification::centred);
            shift += 5;
        }
    }
    else if (currKey == juce::CharPointer_UTF8("E\u266d")) {
        int shift = 0; // for each succ. accidental
        int yshift = 4; // to account for spaces
        for (int i = 0; i < eflat.size(); ++i) {
            juce::Rectangle<int> rect = getLine(eflat[i] - 33);
            juce::Rectangle<int> newBounds;

            if (!checkSpace(eflat[i] - 33)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266d"), newBounds, juce::Justification::centred);

            // MIDI note for bass clef
            rect = getLine(eflat[i] - 33 - 24);
            if (!checkSpace(eflat[i] - 33 - 24)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266d"), newBounds, juce::Justification::centred);
            shift += 5;
        }
    }
    else if (currKey == juce::CharPointer_UTF8("A\u266d")) {
        int shift = 0; // for each succ. accidental
        int yshift = 4; // to account for spaces
        for (int i = 0; i < aflat.size(); ++i) {
            juce::Rectangle<int> rect = getLine(aflat[i] - 33);
            juce::Rectangle<int> newBounds;

            if (!checkSpace(aflat[i] - 33)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266d"), newBounds, juce::Justification::centred);

            // MIDI note for bass clef
            rect = getLine(aflat[i] - 33 - 24);
            if (!checkSpace(aflat[i] - 33 - 24)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266d"), newBounds, juce::Justification::centred);
            shift += 5;
        }
    }
    else if (currKey == juce::CharPointer_UTF8("D\u266d")) {
        int shift = 0; // for each succ. accidental
        int yshift = 4; // to account for spaces
        for (int i = 0; i < dflat.size(); ++i) {
            juce::Rectangle<int> rect = getLine(dflat[i] - 33);
            juce::Rectangle<int> newBounds;

            if (!checkSpace(dflat[i] - 33)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266d"), newBounds, juce::Justification::centred);

            // MIDI note for bass clef
            rect = getLine(dflat[i] - 33 - 24);
            if (!checkSpace(dflat[i] - 33 - 24)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266d"), newBounds, juce::Justification::centred);
            shift += 5;
        }
    }
    else if (currKey == juce::CharPointer_UTF8("G\u266d")) {
        int shift = 0; // for each succ. accidental
        int yshift = 4; // to account for spaces
        for (int i = 0; i < gflat.size(); ++i) {
            juce::Rectangle<int> rect = getLine(gflat[i] - 33);
            juce::Rectangle<int> newBounds;

            if (!checkSpace(gflat[i] - 33)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266d"), newBounds, juce::Justification::centred);

            // MIDI note for bass clef
            rect = getLine(gflat[i] - 33 - 24);
            if (!checkSpace(gflat[i] - 33 - 24)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266d"), newBounds, juce::Justification::centred);
            shift += 5;
        }
    }
    else if (currKey == juce::CharPointer_UTF8("C\u266d")) {
        int shift = 0; // for each succ. accidental
        int yshift = 4; // to account for spaces
        for (int i = 0; i < cflat.size(); ++i) {
            juce::Rectangle<int> rect = getLine(cflat[i] - 33);
            juce::Rectangle<int> newBounds;

            if (!checkSpace(cflat[i] - 33)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266d"), newBounds, juce::Justification::centred);

            // MIDI note for bass clef
            rect = getLine(cflat[i] - 33 - 24);
            if (!checkSpace(cflat[i] - 33 - 24)) {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6, 10, 10 };
            }
            else {
                newBounds = { sigBounds.getX() + shift, rect.getY() - 6 - yshift, 10, 10 };
            }
            g.drawText(juce::CharPointer_UTF8("\u266d"), newBounds, juce::Justification::centred);
            shift += 5;
        }
    }

}

void Stave::resized()
{
    // This method is where you should set the bounds of any child
    // components  your component contains..
    setLines(getLocalBounds()); // Set stave lines first
    for (int i = 0; (int)i < staveNotes.size(); ++i)
    {
        const juce::Rectangle<int>& bounds = getLine(i);
        int width = widthNote;
        int x, y;

        if (i != 0 && i != 3 && i != 7 && i != 48 &&
            i != 51 && i != 55 && i!=27) { // if notes are not ledger, position them well
            x = one.getCentreX() - width / 2;
        }
        else {
            x = bounds.getCentreX() - width/2;
        }

        // logic to deal with line or space
        if (!checkSpace(i)) y = bounds.getCentreY() - (space - thickness) / 2;
        else y = bounds.getCentreY() - space;

        // we need extra space for accidentals so we do 2*width; same with space and y
        staveNotes[i].setBounds(x - width, y - space, 4*width, 4*space);
    }

    
}

void Stave::timerCallback()
{
    // Access keyboard buffer and draw notes
    drawNotes(keyboardBuffer);
}

bool Stave::checkLedger(int note)
{
    // Check if a note is a ledger
    if (note >= 33 && note < 42) {
        return true;
    }
    else if (note >= 88) {
        return true;
    }

    return false;
}

bool Stave::checkSpace(int pos)
{
    // We will consider different ranges
    // false means line; true means space

    if (pos >= 0 && pos < 3) {
        // MIDI range: 33 to 35
        if (pos <= 1) return false;
        else return true;
    }

    if (pos >= 3 && pos < 7) {
        if (pos <= 4) return false;
        else return true;
    }

    if (pos >= 7 && pos < 10) {
        // This is an oddity and deals with semitonal issues
        if (pos == 7) return false;
        else return true;
    }

    if (pos >= 10 && pos < 14) {
        if (pos <= 11) return false;
        else return true;
    }

    if (pos >= 14 && pos < 17) {
        // This is an oddity and deals with semitonal issues
        if (pos == 14) return false;
        else return true;
    }

    if (pos >= 17 && pos < 20) {
        if (pos <= 18) return false;
        else return true;
    }

    if (pos >= 20 && pos < 24) {
        if (pos <= 21) return false;
        else return true;
    }

    if (pos >= 24 && pos < 27) {
        if (pos <= 25) return false;
        else return true;
    }

    if (pos >= 27 && pos < 31) {
        if (pos <= 28) return false;
        else return true;
    }

    if (pos >= 31 && pos < 34) {
        // This is an oddity and deals with semitonal issues
        if (pos == 31) return false;
        else return true;
    }

    if (pos >= 34 && pos < 38) {
        if (pos <= 35) return false;
        else return true;
    }

    if (pos >= 38 && pos < 41) {
        // This is an oddity and deals with semitonal issues
        if (pos == 38) return false;
        else return true;
    }

    if (pos >= 41 && pos < 44) {
        if (pos <= 42) return false;
        else return true;
    }

    if (pos >= 44 && pos < 48) {
        if (pos <= 45) return false;
        else return true;
    }

    if (pos >= 48 && pos < 51) {
        if (pos <= 49) return false;
        else return true;
    }

    if (pos >= 51 && pos < 55) {
        if (pos <= 52) return false;
        else return true;
    }

    // I am assuming anything else should return seventeen
    // But ideally, we need to deal with this upstream and prevent anything higher than 
    // seventeen
    return false;
}

juce::Rectangle<int>& Stave::getLine(int pos)
{
    // We will consider different ranges

    if (pos >= 0 && pos < 3) {
        // MIDI range: 33 to 35
        return one;
    }

    if (pos >= 3 && pos < 7) {
        return two;
    }

    if (pos >= 7 && pos < 10) {
        return three;
    }

    if (pos >= 10 && pos < 14) {
        return four;
    }

    if (pos >= 14 && pos < 17) {
        return five;
    }

    if (pos >= 17 && pos < 20) {
        return six;
    }

    if (pos >= 20 && pos < 24) {
        return seven;
    }

    if (pos >= 24 && pos < 27) {
        return eight;
    }

    if (pos >= 27 && pos < 31) {
        return nine;
    }

    if (pos >= 31 && pos < 34) {
        return ten;
    }

    if (pos >= 34 && pos < 38) {
        return eleven;
    }

    if (pos >= 38 && pos < 41) {
        return twelve;
    }

    if (pos >= 41 && pos < 44) {
        return thirteen;
    }

    if (pos >= 44 && pos < 48) {
        return fourteen;
    }

    if (pos >= 48 && pos < 51) {
        return fifteen;
    }

    if (pos >= 51 && pos < 55) {
        return sixteen;
    }

    // I am assuming anything else should return seventeen
    // But ideally, we need to deal with this upstream and prevent anything higher than 
    // seventeen
    return seventeen;
}

void Stave::drawLedger(int n)
{
    if (n < 8) {
        // bass ledger lines
        if (n < 2){
            if (n == 1 && toggleState) { twof = true; threef = true; }
            else { onef = true; twof = true; threef = true; }

        }

        if (n >=2 && n < 5) {
            if (n==4 && toggleState) { threef = true; }
            else { twof = true; threef = true; }
        }

        if (n >=5 && n <= 7) threef = true;

    }
    else if (n >= 27 && n < 29) {
        // Middle C is n == 27
        if (n == 27) ninef = true;
        else { if (!toggleState) ninef = true; }
    }
    else if (n >= 48) {
        // treble ledger lines
        if (n <= 50) {
            fiff = true;
        }

        if (n >= 51 && n < 54) {
            fiff = true;
            sixf = true;
        }

        if (n >= 54) {
            if (!toggleState && n == 54) { fiff = true; sixf = true; }
            else { fiff = true; sixf = true; sevenf = true; }
        }
    }

    // repaint all the ledger lines
    repaint();
}

void Stave::drawNotes(const std::vector<bool>& keyBuffer)
{
    // clear all the notes
    for (int j = 0; j < staveNotes.size(); ++j)
    {
        staveNotes[j].noteOn = false;
        staveNotes[j].acc = " ";
        staveNotes[j].repaint();
    }

    // clear all ledger lines
    onef = 0;
    twof = 0;
    threef = 0 ;
    ninef = 0;
    fiff = 0;
    sixf = 0;
    sevenf = 0;
    repaint(); // clear the ledger lines here too from repaint

    // Draw notes in keyBuffer
    for (int i = 33; i <= 88; ++i)
    {
        // lowest note on 88-key piano is 21; highest note is 108
        // But for now, let's consider 33 to 88
        
        // Get note and draw it
        if (keyBuffer[i]) {
            // Get line to draw note
 
            // for three ledger lines: lowest note is 33 and highest is 88 (MIDI pitch)
            // draw note at line or in space adj. to line
            int n = i - 33;

            // if ledger is on, draw the proper ledger lines
            drawLedger(n);

            // draw note
            // get accidental for note
            juce::String accidental = getAccidental(currKey, i);
            staveNotes[n].noteOn = true;
            staveNotes[n].acc = accidental;
            staveNotes[n].repaint();
        }
        
    }
}

void Stave::changeBounds()
{
    std::vector<int> midiPitches{34, 37, 39, 42, 44, 46, 49, 51, 54, 56, 58, 61, 63, 
    66, 68, 70, 73, 75, 78, 80, 82, 85, 87};

    for (int elem : midiPitches) {
        int pos = elem - 33; // Get the distance from midi 33
        if (toggleState) {
            // true means flats are on
            juce::Rectangle<int> currBounds = staveNotes[pos].getBounds();

            // dist is divided by four since our width was mult by 4 in resized
            int dist = (currBounds.getCentreY() - currBounds.getY())/4; 
            juce::Rectangle<int> newBounds;

            // Check if pos was initially on a space in order to place it well
            if (checkSpace(pos)) { newBounds = currBounds.withY(currBounds.getY() - dist); }
            else {newBounds = currBounds.withY(currBounds.getY() - dist - thickness); }

            // These changes are not supposed to be made but had to due the accidental logic 
            // in resized
            auto x = newBounds.toFloat().getX();
            auto y = newBounds.toFloat().getY();
            auto width = newBounds.toFloat().getWidth();
            auto height = newBounds.toFloat().getHeight();
            staveNotes[pos].setBounds(newBounds);
        }
        else {
            // sharps are on
            juce::Rectangle<int> currBounds = staveNotes[pos].getBounds();

            // dist is divided by four since our width was mult by 4 in resized
            int dist = (currBounds.getCentreY() - currBounds.getY())/4;
            juce::Rectangle<int> newBounds;

            if (checkSpace(pos)) { newBounds = currBounds.withY(currBounds.getY() + dist); }
            else{ newBounds = currBounds.withY(currBounds.getY() + dist + thickness);}

            staveNotes[pos].setBounds(newBounds);
        }
    }
}

void Stave::setLines(const juce::Rectangle<int>& bounds)
{
    // Width and height of Grand Stave
    auto widthStave = bounds.getWidth(); // width of each stave is same as grand
    auto heightGrand = bounds.getHeight();
    space = heightGrand/20; // eighteen spaces and distance btw staves as 2*space; hence, 19
    distStave = 4*space;

    // Ledgers line above treble clef
    seventeen = { bounds.getX() + xNotes, bounds.getY() + space, widthLedger, thickness };
    sixteen = { bounds.getX() + xNotes, bounds.getY() + 2*space, widthLedger, thickness };
    fifteen = { bounds.getX() + xNotes, bounds.getY() + 3*space, widthLedger, thickness };

    // Treble Clef
    fourteen = { bounds.getX(), bounds.getY() + 4*space, widthStave, thickness };
    thirteen = { bounds.getX(), bounds.getY() + 5*space, widthStave, thickness };
    twelve = { bounds.getX(), bounds.getY() + 6* space, widthStave, thickness };
    eleven = { bounds.getX(), bounds.getY() + 7 * space, widthStave, thickness };
    ten = { bounds.getX(), bounds.getY() + 8 * space, widthStave, thickness };

    // Middle C
    nine = { bounds.getX() + xNotes, bounds.getY() + 9* space, 
        widthLedger, thickness};

    // Bass Clef
    int bs = static_cast<int>(distStave);
    eight = { bounds.getX(), bounds.getY() + bs + 8*space, widthStave, thickness };
    seven = { bounds.getX(), bounds.getY() + bs + 9*space, widthStave, thickness };
    six = { bounds.getX(), bounds.getY() + bs + 10*space, widthStave, thickness };
    five = { bounds.getX(), bounds.getY() + bs + 11*space, widthStave, thickness };
    four = { bounds.getX(), bounds.getY() + bs + 12*space, widthStave, thickness };

    // Ledgers line below bass clef
    three = { bounds.getX() + xNotes, bounds.getY() + bs + 13*space, widthLedger, thickness };
    two = { bounds.getX() + xNotes, bounds.getY() + bs + 14* space, widthLedger, thickness };
    one = { bounds.getX() + xNotes, bounds.getY() + bs + 15 * space, widthLedger, thickness };
}


juce::String Stave::getAccidental(juce::String key, int note)
{
    // Returns an Accidental
    if (key == "C") {
        // convert note to its corresponding pitch class
        int p = note % 12;

        if (toggleState) {
            // Flats is On
            if (p == 1 || p == 3 || p == 6 || p == 8 || p == 10) {
                return juce::CharPointer_UTF8("\u266d");
            }
            return " ";
        }
        else {
            // Sharps is On
            if (p == 1 || p == 3 || p == 6 || p == 8 || p == 10) {
                return juce::CharPointer_UTF8("\u266f");
            }
            return " ";
        }
    }
    else if (key == "G")
    {
        int p = note % 12;
        
        if (p == 1 || p == 3 || p == 8 || p == 10) {
            return juce::CharPointer_UTF8("\u266f");
        }
        else if (p == 5) {
            return juce::CharPointer_UTF8("\u266e");
        }
        else {
            return " ";
        }
        
    }
    else if (key == "D") {
        int p = note % 12;

        if (p == 3 || p == 8 || p == 10) {
            return juce::CharPointer_UTF8("\u266f");
        }
        else if (p == 5 || p == 0) {
            return juce::CharPointer_UTF8("\u266e");
        }
        else {
            return " ";
        }
    }
    else if (key == "A") {
        int p = note % 12;

        if (p == 3 || p == 10) {
            return juce::CharPointer_UTF8("\u266f");
        }
        else if (p == 5 || p == 0 || p == 7) {
            return juce::CharPointer_UTF8("\u266e");
        }
        else {
            return " ";
        }
    }
    else if (key == "E") {
        int p = note % 12;

        if (p == 10) {
            return juce::CharPointer_UTF8("\u266f");
        }
        else if (p == 5 || p == 7 || p == 0 || p == 2) {
            return juce::CharPointer_UTF8("\u266e");
        }
        else {
            return " ";
        }
    }
    else if (key == "B") {
        int p = note % 12;

        if (p == 5 || p == 0 || p == 2 || p == 7 || p == 9) {
            return juce::CharPointer_UTF8("\u266e");
        }
        else {
            return " ";
        }
    }
    else if (key == "F#") {
        int p = note % 12;

        if (p == 5 || p == 7 || p == 9 || p == 0 || p == 2 || p == 4) {
            return juce::CharPointer_UTF8("\u266e");
        }
        else {
            return " ";
        }
    }
    else if (key == "C#") {
        int p = note % 12;

        if (p == 5 || p == 2 || p == 4 || p == 7 || p == 9 || p == 11) {
            return juce::CharPointer_UTF8("\u266e");
        }
        else {
            return " ";
        }
    }
    else if (key == "F") {
        int p = note % 12;

        if (p == 1 || p == 3 || p == 6 || p == 8) {
            return juce::CharPointer_UTF8("\u266d");
        }
        else if (p == 11) {
            return juce::CharPointer_UTF8("\u266e");
        }
        else {
            return " ";
        }
    }
    else if (key == juce::CharPointer_UTF8("B\u266d")) {
        int p = note % 12;

        if (p == 1 || p == 8 || p == 6) {
            return juce::CharPointer_UTF8("\u266d");
        }
        else if (p == 11 || p == 4) {
            return juce::CharPointer_UTF8("\u266e");
        }
        else {
            return " ";
        }
    }
    else if (key == juce::CharPointer_UTF8("E\u266d")) {
        int p = note % 12;

        if (p == 1 || p == 6) {
            return juce::CharPointer_UTF8("\u266d");
        }
        else if (p == 11 || p == 4 || p == 9) {
            return juce::CharPointer_UTF8("\u266e");
        }
        else {
            return " ";
        }
    }
    else if (key == juce::CharPointer_UTF8("A\u266d")) {
        int p = note % 12;

        if (p == 6) {
            return juce::CharPointer_UTF8("\u266d");
        }
        else if (p == 11 || p == 4 || p == 9 || p == 2) {
            return juce::CharPointer_UTF8("\u266e");
        }
        else {
            return " ";
        }
    }
    else if (key == juce::CharPointer_UTF8("D\u266d")) {
        int p = note % 12;

        if (p == 11 || p == 4 || p == 9 || p == 2 || p == 7) {
            return juce::CharPointer_UTF8("\u266e");
        }
        else {
            return " ";
        }
    }
    else if (key == juce::CharPointer_UTF8("G\u266d")) {
        int p = note % 12;

        if (p == 11 || p == 4 || p == 9 || p == 2 || p == 7 || p == 0) {
            return juce::CharPointer_UTF8("\u266e");
        }
        else {
            return " ";
        }
    }
    else if (key == juce::CharPointer_UTF8("C\u266d")) {
        int p = note % 12;
        if (p == 11 || p == 4 || p == 9 || p == 2 || p == 7 || p == 0 || p == 5) {
            return juce::CharPointer_UTF8("\u266e");
        }
        else {
            return " ";
        }
    }
}

void Stave::setKey(juce::String key)
{
    currKey = key;
    repaint();
}