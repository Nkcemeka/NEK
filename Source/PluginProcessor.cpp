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
                       ), noteBuffer(12, false), keyboardBuffer(128, false),
    c{ 512, 44100 }, apvts{*this, nullptr, "Parameters", createParameterLayout()}
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

    // setup Parameters
    castParameter(apvts, ParameterID::mode, modeParam);
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
    currentSampleRate = sampleRate; 
    c.setSamplingFrequency(currentSampleRate);
    //c.setChromaCalculationInterval((int)(currentSampleRate*0.01)); // default: 8192 (about 17% of sample rate of 48k)
    c.setChromaCalculationInterval(480);
    prevTime = juce::Time::getMillisecondCounterHiRes();
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
    // We need to process audio if audio mode is on
    const float* channelData = buffer.getReadPointer(0);
    int numSamples = buffer.getNumSamples();

    if (modeParam->get()) { 
        handleAudioChord(numSamples, channelData); }
    else { handleMidiChord(buffer, midiMessages); }
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
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
}

void NEKAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType())) {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
        //parametersChanged.store(true)l
    }
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

void NEKAudioProcessor::handleMidiChord(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    //Clear the buffer. We don't need audio
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

void NEKAudioProcessor::handleAudioChord(int numSamples, const float* channelData) {
    // convert channelData to double and calculate max amp. and peak amp.
    std::vector<double> doubleChannelData(numSamples);
    double maximum = 0;
    double rmsThresh = 0.001;
    double rms = 0;
    double rmsTotal = 0;

    for (int i = 0; i < numSamples; ++i) {
        doubleChannelData[i] = static_cast<double>(channelData[i]);
        maximum = juce::jmax(maximum, abs(doubleChannelData[i]));
        rmsTotal += pow(channelData[i], 2);
    }

    rms = rmsTotal / numSamples;


    if (maximum >= 0.001) { // To ensure the signal isn't silent
        if (rms >= rmsThresh) {
            // Convert audio samples to chromagram
            //Chromagram c(numSamples, currentSampleRate);
            c.setInputAudioFrameSize(numSamples);
            c.processAudioFrame(doubleChannelData);

            if (c.isReady())
            {
                //if (juce::Time::getMillisecondCounterHiRes() - prevTime <= 500) { return; };
                std::vector<double> chroma = c.getChromagram();

                // do something with the chromagram here
                chordDetector.detectChord(chroma);

                juce::String root;
                if (chordDetector.quality == 0 && chordDetector.intervals == 0) {
                    rootNote = chordDetector.rootNote % 12;
                    if (flats) { root = noteMapF[rootNote]; }
                    else { root = noteMap[chordDetector.rootNote % 12]; };

                    ch = root + "Min";
                }
                else if (chordDetector.quality == 1 && chordDetector.intervals == 0) {
                    rootNote = chordDetector.rootNote % 12;
                    if (flats) { root = noteMapF[rootNote]; }
                    else { root = noteMap[chordDetector.rootNote % 12]; };

                    ch = root + "Maj";
                }
                else if (chordDetector.quality == 2 && chordDetector.intervals == 2) {
                    rootNote = chordDetector.rootNote % 12;
                    if (flats) { root = noteMapF[rootNote]; }
                    else { root = noteMap[chordDetector.rootNote % 12]; };

                    ch = root + "Sus2";
                }
                else if (chordDetector.quality == 2 && chordDetector.intervals == 4) {
                    rootNote = chordDetector.rootNote % 12;
                    if (flats) { root = noteMapF[rootNote]; }
                    else { root = noteMap[chordDetector.rootNote % 12]; };

                    ch = root + "Sus4";
                }
                else if (chordDetector.quality == 1 && chordDetector.intervals == 7) {
                    rootNote = chordDetector.rootNote % 12;
                    if (flats) { root = noteMapF[rootNote]; }
                    else { root = noteMap[chordDetector.rootNote % 12]; };

                    ch = root + "Maj7";
                }
                else if (chordDetector.quality == 0 && chordDetector.intervals == 7) {
                    rootNote = chordDetector.rootNote % 12;
                    if (flats) { root = noteMapF[rootNote]; }
                    else { root = noteMap[chordDetector.rootNote % 12]; };

                    ch = root + "Min7";
                }
                else if (chordDetector.quality == 3 && chordDetector.intervals == 7) {
                    rootNote = chordDetector.rootNote % 12;
                    if (flats) { root = noteMapF[rootNote]; }
                    else { root = noteMap[chordDetector.rootNote % 12]; };

                    ch = root + "7";
                }


                prevTime = juce::Time::getMillisecondCounterHiRes();
            }
        }
    }
    else { ch = " "; rootNote = -1; }
}

juce::AudioProcessorValueTreeState::ParameterLayout NEKAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // add mode button to layout
    layout.add(std::make_unique<juce::AudioParameterBool>(
        ParameterID::mode,
        "Audio/MIDI Mode",
        false
    ));

    return layout;
}