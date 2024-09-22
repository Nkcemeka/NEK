/*
  ==============================================================================

    Analyzer.cpp
    Created: 20 Aug 2024 9:06:32am
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#include "Analyzer.h"

/*
    Function definitions for declarations in Analyzer.h
*/

int dist(int pos1, int pos2) {
    // Gets the distance between two notes 
    // pos is such that: C = 0, C#/Db = 1 etc
    int distance = std::abs(pos1 - pos2);
    return distance;
}

// Function to get the chord name
std::string getChordName(std::vector<bool>& noteBuffer,
    std::vector<bool>& keyBuffer, std::map<int, std::string>& intervalMap,
    std::map<std::string, std::string>& chordMap, std::map<int, std::string>& noteMap,
    std::map<int, std::string>& noteMapF, std::map<std::string, std::string>& reverseMap,
    bool flats, std::atomic<int>& rootNote) {
    // Clear the noteBuffer; this is important in order
    // to prevent unexpected behaviour when holding down
    // notes and adding others on top
    std::fill(noteBuffer.begin(), noteBuffer.end(), 0);

    // Populate the noteBuffer from the current state of the keyboard Buffer
    for (int i = 0; i < 128; ++i) {
        int midiNum = (i) % 12;
        noteBuffer[midiNum] = noteBuffer[midiNum] + keyBuffer[i];
    }

    // This vector stores the position of notes that are currently active
    std::vector<int> notePos = {};
    for (int i = 0; i < 12; ++i) {
        if (noteBuffer[i]) {
            notePos.push_back(i);
        }
    }

    // Assume the first note in notePos is the root
    int root; // declare the root
    int distance;
    std::string chordName = "";

    for (int i = 0; i < (int)notePos.size(); ++i) {
        std::string intervals = "";
        root = notePos[i];

        // Modulo sign ensures we loop to beginning of loop if
        // necessary. And j!= root means we skip the root. This 
        // is critical for the algo to work and helps in dealing
        // with inversions
        for (int j = i + 1; j != i; j = (j + 1) % notePos.size()) {
            if (j == (int)notePos.size()) {
                j = 0;
            }
            distance = dist(root, notePos[j]);
            std::string revInterval = "";

            if (j < i) {
                revInterval += reverseMap[intervalMap[distance]];
                intervals += revInterval;
            }
            else {
                intervals += intervalMap[distance];
            }

        }

        // Get a match from the database
        chordName = chordName + chordMap[intervals];

        if (chordName != "") {
            rootNote = root;
            if (flats) {
                return noteMapF[root] + chordName;
            }
            return noteMap[root] + chordName;
        }
    }

    rootNote = -1;
    return "";
}
