/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Analyzer.h"

//==============================================================================
NEKAudioProcessor::NEKAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), noteBuffer(12, false), keyboardBuffer(128, false)
#endif
{
    intervalMap = intMAP;
    chordMap = chMAP;
    noteMap = NMAP;
    noteMapF = NMAPF;
    reverseMap = revMAP;
    flats = 0;
    ch = "";
    noteNum = 0;
}

NEKAudioProcessor::~NEKAudioProcessor()
{
}

//==============================================================================
const juce::String NEKAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NEKAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NEKAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NEKAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NEKAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NEKAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NEKAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NEKAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NEKAudioProcessor::getProgramName (int index)
{
    return {};
}

void NEKAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NEKAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void NEKAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NEKAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NEKAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Clear the buffer. We don't need audio
    buffer.clear();
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        noteNum = message.getNoteNumber() % 12;

        // Update the keyboardBuffer
        if (message.isNoteOn()) {
            keyboardBuffer[message.getNoteNumber()] = true;
        }
        else if (message.isNoteOff()) {
            keyboardBuffer[message.getNoteNumber()] = false;
        }
    }
    handleChord(noteBuffer, keyboardBuffer, intervalMap, chordMap, noteMap,
        noteMapF, reverseMap, flats, rootNote);
}

//==============================================================================
bool NEKAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NEKAudioProcessor::createEditor()
{
    return new NEKAudioProcessorEditor (*this);
}

//==============================================================================
void NEKAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NEKAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NEKAudioProcessor();
}


void NEKAudioProcessor::handleChord(std::vector<bool>& noteBuffer,
    std::vector<bool>& keyBuffer, std::map<int, juce::String>& intervalMap,
    std::map<juce::String, juce::String>& chordMap, std::map<int, juce::String>& noteMap,
    std::map<int, juce::String>& noteMapF, std::map<juce::String, juce::String>& reverseMap,
    bool flats, std::atomic<int>& rootNote)
{
    // Display chord name or depict as notes if no detection
    ch = getChordName(noteBuffer, keyBuffer, intervalMap, chordMap, noteMap,
        noteMapF, reverseMap, flats, rootNote);

    if (ch == "") {
        // Show notes if chord not detected
        for (int i = 0; i < 128; ++i) {
            if (keyboardBuffer[i]) {
                if (flats) {
                    ch = ch + noteMapF[i % 12] + ", ";
                }
                else {
                    ch = ch + noteMap[i % 12] + ", ";
                }
            }
        }
        ch = ch + "";
        ch = ch.substring(0, ch.length() - 2);
    };
}