/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//==============================================================================
/**
*/
class NEKAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    NEKAudioProcessor();
    ~NEKAudioProcessor() override;

    //==============================================================================
    // definitions for chord naming or analysis
    int noteNum;
    juce::String note;
    juce::String ch;
    std::vector<bool> noteBuffer; // Contains the state of all active pitch classes (0 - 11)
    std::vector<bool> keyboardBuffer; // Contains the state of all active MIDI notes (0 - 127)
    juce::MidiKeyboardState keyboardState;
    std::map<int, std::string> intervalMap;
    std::map<std::string, std::string> chordMap;
    std::map<int, std::string> noteMap; // Map for sharps
    std::map<int, std::string> noteMapF; // Map for flats
    std::map<std::string, std::string> reverseMap; // Map for intervallic inversions
    bool flats; // Flag to show if the user wants chord names as flats or sharps


    // default declarations from JUCE
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    std::atomic <int> rootNote{ -1 };

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NEKAudioProcessor)
};
