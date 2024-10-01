/*
  ==============================================================================

    Stave.h
    Created: 23 Sep 2024 2:50:18pm
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "NoteHead.h"

//==============================================================================
/*
*/
class Stave  : public juce::Component, private juce::Timer
{
public:
    Stave(std::vector<bool>& kb);
    ~Stave() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setLines(const juce::Rectangle<int>& bounds);
    void drawNotes(const std::vector<bool>&);
    void timerCallback() override;
    bool checkLedger(int note);
    bool checkSpace(int note);
    juce::Rectangle<int>& getLine(int pos);
    bool toggleState{ 0 }; // true means flats are on and sharps are off
    void changeBounds();
    void drawLedger(int);
    juce::String getAccidental(juce::String key, int note);
   

    // Setup flags for ledger lines
    bool onef{ 0 };
    bool twof{ 0 };
    bool threef{ 0 };
    bool ninef{ 0 };
    bool fiff{ 0 };
    bool sixf{ 0 };
    bool sevenf{ 0 };

    // Lines
    juce::Rectangle<int> one; // first ledger line counting from the bass
    juce::Rectangle<int> two;
    juce::Rectangle<int> three;
    juce::Rectangle<int> four; // first bass line
    juce::Rectangle<int> five;
    juce::Rectangle<int> six;
    juce::Rectangle<int> seven;
    juce::Rectangle<int> eight; // last bass line
    juce::Rectangle<int> nine; // middle C
    juce::Rectangle<int> ten; // first treble line
    juce::Rectangle<int> eleven; 
    juce::Rectangle<int> twelve;
    juce::Rectangle<int> thirteen;
    juce::Rectangle<int> fourteen; // last treble line
    juce::Rectangle<int> fifteen; // first ledger line above treble
    juce::Rectangle<int> sixteen;
    juce::Rectangle<int> seventeen; // last ledger line

    // key signature
    void setKey(juce::String);

private:
    int thickness; // line thickness of stave
    float distStave; // distance between staves
    int widthLedger; // width of ledger lines
    int xNotes; // X position of notes in Stave
    int widthNote; // width of each note head
    int space; // size of space or distance between Stave lines
    std::vector<NoteHead> staveNotes; // 56 note heads including those on ledger lines
    std::vector<bool>& keyboardBuffer;

    // key signature
    juce::String currKey{ "C"}; // currKey

    // Store midi pitch position for bass and treble Clef
    std::vector<int> gkey{ 77 };
    std::vector<int> d{77, 72};
    std::vector<int> a{77, 72, 79};
    std::vector<int> e{ 77, 72, 79, 74};
    std::vector<int> b{77, 72, 79, 74, 69};
    std::vector<int> fsharp{77, 72, 79, 74, 69, 76};
    std::vector<int> csharp{ 77, 72, 79, 74, 69, 76, 71};
    std::vector<int> f{71};
    std::vector<int> bflat{71, 76};
    std::vector<int> eflat{71, 76, 69};
    std::vector<int> aflat{71, 76, 69, 74};
    std::vector<int> dflat{ 71, 76, 69, 74, 67};
    std::vector<int> gflat{71, 76, 69, 74, 67, 72};
    std::vector<int> cflat{71, 76, 69, 74, 67, 72, 65};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Stave)
};
