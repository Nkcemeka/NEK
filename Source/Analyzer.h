/*
  ==============================================================================

    Analyzer.h
    Created: 12 Aug 2024 5:31:19pm
    Author:  Chukwuemeka L. Nkama
    Description: Contains logic for getting the chord names

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

// This is an interval mapping. It maps 
// integers (semitone distances) to 
// intervals
#define intMAP \
    {{0, "Unison"}, \
    { 1, "m2" }, \
    { 2, "M2" }, \
    { 3, "m3" }, \
    { 4, "M3" }, \
    { 5, "P4" }, \
    { 6, "Tritone"}, \
    { 7, "P5" },\
    { 8, "m6" },  \
    { 9, "M6" }, \
    { 10, "m7" }, \
    { 11, "M7" },\
    { 12, "Octave" }}

// This is a mapping that returns the inversion
// of intervals denoted in the map's key
#define revMAP \
    {{"Unison", "Unison"}, \
    { "M7", "m2" }, \
    { "m7", "M2" }, \
    { "M6", "m3" }, \
    { "m6", "M3" }, \
    { "P5", "P4" }, \
    { "Tritone", "Tritone"}, \
    { "P4", "P5" },\
    { "M3", "m6" },  \
    { "m3", "M6" }, \
    { "M2", "m7" }, \
    { "m2", "M7" },\
    { "Octave", "Octave" }}

// This maps the intervals that form a chord
// to the name of the chord
#define chMAP {\
    {"M3P5", "Maj"},\
    { "m3P5", "m" }, /*Minor Chords*/ \
    { "M3Tritone", "+" }, /*Aug Chord*/ \
    { "m3Tritone", "o" }, /*Dim Chord */ \
    { "M3P5m7", "7" }, /*Dom 7 Chord*/ \
    { "M3P5M7", "M7" }, /*Major 7th*/ \
    { "M3M7", "M7(no5)"}, /* Major 7 no5 */ \
    { "m3P5M7", "mM7" }, /* Minor Major Seventh */ \
    { "m3P5m7", "m7" }, /* Minor 7th */ \
    { "m3m7", "m7(no5)"}, /* Minor 7 no5 */ \
    { "M3m6M7", "+M7" }, /* Aug Maj 7*/ \
    { "m3Tritonem7", "m7b5" }, /* Half Dim 7th */\
    { "m3TritoneM6","o7"}, /* Dim 7th */\
    { "M3Tritonem7", "7b5"}, /* 7th b5 */\
    { "M3P5M6", "M6"}, /* Major Sixth */ \
    { "m3P5M6", "m6"}, /* Minor Sixth */\
    { "M2M3P5M7", "M9"}, /* Major Ninth */\
    { "M2M3M7", "M9(no5)"}, /* Major 9 no5 */ \
    { "M2M3P5", "Madd9"}, /* Add 9 */ \
    { "M2M3P5m7", "9"}, /* Dominant 9th */\
    { "M2m3P5M7", "mM9"}, /* MinMaj 9th */\
    { "M2m3P5m7", "m9"}, /* Minor 9th */ \
    { "M2m3m7", "m9(no5)"}, /* Minor 9 no5 */ \
    { "M2m3P5", "madd9"}, /* Minor Add 9 */ \
    { "M2M3m6M7", "+M9"}, /* AugMaj 9th */\
    { "M2M3m6m7", "9#5"}, /* AugDom 9th */\
    { "M2m3Tritonem7", "m9b5"}, /* Half Dim 9th */\
    { "m2m3Tritonem7", "m7b5b9"}, /* Half Dim Min 9th */\
    { "M2m3TritoneM6", "o9"}, /* Dim 9th */\
    { "m2m3TritoneM6", "ob9"}, /* Dim Min 9th */\
    { "M2M3P4P5m7", "11"}, /* Dom 11th */\
    { "M2M3P4P5M7", "M11"}, /* Maj 11th */\
    { "M2m3P4P5M7", "mM11"}, /* Min Maj 11th */\
    { "M2m3P4P5m7", "m11"}, /* Min 11th */\
    { "M2M3P4m6M7", "+M11"}, /* Aug Maj 11th */\
    { "M2M3P4m6m7", "+11"}, /* Aug 11th */\
    { "M2m3P4Tritonem7", "m11b5"}, /* Half Dim 11th */\
    { "M2m3P4TritoneM6", "o11"}, /* Dim 11th */\
    { "M2M3P4P5M6M7", "Maj13"}, /* Maj 13th */\
    { "M2M3P4P5M6m7", "13"}, /* Dom 13th */\
    { "M2m3P4P5M6M7", "mM13"}, /* Min Maj 13th */\
    { "M2m3P4P5M6m7", "m13"}, /* Min 13th */\
    { "M2M3P4m6M6M7", "+M13"}, /* Aug Maj 13th */\
    { "M2M3P4m6M6m7", "+13"}, /* Aug Dom 13th */\
    { "M2m3P4TritoneM6m7", "m13b5"}, /* Half Dim 13th */\
    { "M2P5", "Sus2"}, /* Sus2 */\
    { "P4P5", "Sus4"}, /* Sus4 */\
    { "P4P5m7", "7Sus4"}, /* Sus 4th 7th*/\
};

// This is a note mapping. It assigns each pitch class in
// the 12TET system an integer. Accidentals here are rep.
// by sharps
#define NMAP {\
        {0, "C"},\
        { 1, "C#" },\
        { 2, "D" },\
        { 3, "D#" },\
        { 4, "E" },\
        { 5, "F" },\
        { 6, "F#" },\
        { 7, "G" },\
        { 8, "G#" },\
        { 9, "A" },\
        { 10, "A#" },\
        { 11, "B" },\
    };

// This is a note mapping. It assigns each pitch class in
// the 12TET system an integer. Accidentals here are rep.
// by flats
#define NMAPF {\
        {0, "C"},\
        { 1, "Db" },\
        { 2, "D" },\
        { 3, "Eb" },\
        { 4, "E" },\
        { 4, "Fb" },\
        { 5, "F" },\
        { 6, "Gb" },\
        { 7, "G" },\
        { 8, "Ab" },\
        { 9, "A" },\
        { 10, "Bb" },\
        { 11, "B" },\
        { 11, "Cb" }\
 };


// Declare functions for chord analysis
int dist(int pos1, int pos2); // Calculate the distance between notes in semitones
std::string getChordName(std::vector<bool>& noteBuffer,
    std::vector<bool>& keyBuffer, std::map<int, std::string>& intervalMap,
    std::map<std::string, std::string>& chordMap, std::map<int, std::string>& noteMap,
    std::map<int, std::string>& noteMapF, std::map<std::string, std::string>& reverseMap,
    bool flats);