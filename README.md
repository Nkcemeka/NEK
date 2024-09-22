![alt text](https://raw.githubusercontent.com/Nkcemeka/NEK/main/nek.png "NEK")


# NEK - A MIDI Chord Analyzer Plugin
NEK is an open source MIDI chord analyzer plugin. It takes in MIDI notes and outputs a chord name. The algorithm has a database for intervals and what intervals make up the chords. By projecting notes into different pitch classes, it uses an elegant but subtle algorithm to deduce the note and get the appropriate chord. This is all the more remarkable as the database is extensible. Put in the intervals that make up your chord and you are good to go! NEK has also a Circle of Fourths/Fifths visualizer. This can make it useful to identify harmonic patterns while playing. I might include functionality for choosing the key and turning the visualizer to show the progression on the circle in numbers. If I do this, I won't go ahead with the idea of note rendering, but might leave that off to another project. 

## Demo Video
You can check out a DEMO video of NEK on my LinkedIn page [here](https://www.linkedin.com/posts/chukwuemeka-nkama-0203611b2_recently-i-needed-a-chord-analyzer-plugin-activity-7230562767424917504-Ct0K).

## Todo
---
```
- Note Rendering Capabilities
- Automated Chord Detection using Deep Learning & Chromograms
```
---

## Minimum System Requirements
#### Building NEK
- C++ Standard: 17
- Windows: Visual Studio 2019 or Higher
Note that NEK can be built for Linux and Android as well. In my case, I built it on Windows without issues


## Contributing
If you want to contribute, please send me a pull request! If you want to also modify the code for your use, feel free to do so. A shoutout will be appreciated as well :-).

## Licensing
To the best of my knowledge, you can build this plugin for your own personal use. I have not made a release yet, and I am not sure I would. I might change my mind once I fully understand the idiosyncrasies that come with Licensing with JUCE. P.S: I heard I might have to use a GPL license to make the code open source. Thinking about it!! It is sad I don't have the option of an MIT License as well.
