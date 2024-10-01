/*
  ==============================================================================

    CircleofFifths.cpp
    Created: 21 Sep 2024 4:45:52pm
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CircleofFifths.h"

//==============================================================================
CircleofFifths::CircleofFifths() : notesVec(12)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // Setup all the notes to put in a circle
    // Starting with SHARPS since that is the default
    // We will change the keys if the mode is changed
    notesVec[0].setKey("C");
    notesVec[1].setKey("F");
    notesVec[2].setKey("A#");
    notesVec[3].setKey("D#");
    notesVec[4].setKey("G#");
    notesVec[5].setKey("C#");
    notesVec[6].setKey("F#");
    notesVec[7].setKey("B");
    notesVec[8].setKey("E");
    notesVec[9].setKey("A");
    notesVec[10].setKey("D");
    notesVec[11].setKey("G");

    // Make all the notes visible
    addAndMakeVisible(&notesVec[0]);
    addAndMakeVisible(&notesVec[1]);
    addAndMakeVisible(&notesVec[2]);
    addAndMakeVisible(&notesVec[3]);
    addAndMakeVisible(&notesVec[4]);
    addAndMakeVisible(&notesVec[5]);
    addAndMakeVisible(&notesVec[6]);
    addAndMakeVisible(&notesVec[7]);
    addAndMakeVisible(&notesVec[8]);
    addAndMakeVisible(&notesVec[9]);
    addAndMakeVisible(&notesVec[10]);
    addAndMakeVisible(&notesVec[11]);
}

CircleofFifths::~CircleofFifths()
{
}

void CircleofFifths::paint(juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    auto bounds = getLocalBounds();
    auto cx = bounds.getCentreX();
    auto cy = bounds.getCentreY();

    // Draw an outer and inner circle
    g.setColour(juce::Colours::black);

    // we reduced outer circle by thickness to allow circle not get chopped off
    g.drawEllipse(bounds.toFloat().reduced(5), 5); // outer circle
    g.drawEllipse(bounds.toFloat().reduced(56), 1); // inner circle
    g.fillEllipse(bounds.toFloat().reduced(56)); // Make inner circle black
}

void CircleofFifths::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    auto bounds = getLocalBounds();
    auto cx = bounds.getCentreX();
    auto cy = bounds.getCentreY();
    float theta = -90.0f; // starting angle and moving clockwise
    float rot = 30; // Each key is 30 deg from each other
    float rad = 70;
    const auto PI = 3.141592653590;

    // setup button width and height
    float bw = bounds.getWidth() / 10 + 25;
    float bh = bw;

    for (auto i = 0; i < 12; ++i)
    {
        // Get current button
        CircleButton& button = notesVec[i];

        // Now, how do I get the midpoint of the button
        // Using vectors on a piece of paper, I got the following...
        // a = cx + rcos(theta) and b = cy + rsin(theta) where (a,b) is point on circle
        float a = cx + rad * std::cos(theta * PI / 180);
        float b = cy + rad * std::sin(theta * PI / 180);

        // (a,b) is button position. 
        button.setBounds(a - bw / 2, b - bw / 2, bw, bh);
        theta += rot;
    }
}

void CircleofFifths::circleUpdate(int root)
{
    // Change color and call repaint

    for (int i = 0; i < 12; ++i) {
        // root is in 12TET pitch class format, so we need to convert
        // to circleOfFifths format.
        // eqn for this is: posCircle = (pitchClass + 4*pitchClass)%12
        int p = (i + 4 * i) % 12; // p is pitchClass
        if (i == root) { 
            notesVec[p].color = juce::Colours::green; 
            notesVec[p].textColor = juce::Colours::white;
        }
        else { 
            notesVec[p].color = juce::Colours::white; 
            notesVec[p].textColor = juce::Colours::black;
        }
        notesVec[p].repaint();
    }

}