<img src="./nek.png" width="800" />


# NEK - A MIDI-Audio Chord Recognition Plugin
NEK is an open source MIDI-Audio chord analyzer and is essentially a music education software. NEK can work as a software app or VST plugin. It takes in MIDI notes and outputs a chord name. The algorithm has a database for intervals and what intervals make up the chords. By projecting notes onto different pitch classes, it uses an elegant but subtle algorithm to deduce the note and get the appropriate chord. This is all the more remarkable as the database is extensible. Put in the intervals that make up your chord and you are good to go! NEK has also a Circle of Fourths/Fifths visualizer. This can make it useful to identify harmonic patterns while playing. NEK can also accept audio by pressing the "mode" button. It implements a DSP algo from [Adam Stark's PhD Thesis](https://github.com/adamstark/Chord-Detector-and-Chromagram). The algorithm is a brilliant one. However, it needs lots of work. It is susceptible to ghost notes and there are some other issues. I intend to replace this with an AI functionality in the future. Hence, see this feature as experimental.  Lastly, NEK comes with notation rendering functionality! As you press notes on the keyboard, they are highlighted on a Grand Stave according to the key you select. 

## Demo Video
You can check out an early DEMO video of NEK on my LinkedIn page [here](https://www.linkedin.com/posts/chukwuemeka-nkama-0203611b2_recently-i-needed-a-chord-analyzer-plugin-activity-7230562767424917504-Ct0K). I will make a new video soon.

## Todo
---
```
Automated Chord Detection using Deep Learning.
```
---

## Automated Chord Detection from Audio
This is an experimental feature based on DSP and is not highly accurate. However to use this, press the mode button. Once the LED turns red, it means you are in audio mode. To switch back to MIDI, press the button again. And don't worry about saving your state in the DAW. NEK remembers its previous state in a DAW enviroment!

## Oddities
There are several odd things to note about NEK:
- Musical Notation is a very complex process from what I have learnt; there are so many potential sources of errors.
- Due to the complexity of musical notation, NEK has some weird behaviours. For example, it might render a natural instead of a flat or sharp. The rendering is correct, but it might look weird depending on the context of what you are playing. Be rest assured that this is not an error but a design decision. If I could rebuild the plugin from scratch, I might chage my approach.
- NEK is limited to three ledger lines above and below the Grand Stave. So yeah, play your MIDI notes in that region :-)

## Minimum System Requirements
#### Building NEK
- C++ Standard: 17
- Windows: Visual Studio 2019 or Higher
Note that NEK can be built for MAC and Linux as well. In my case, I built it on Windows using Projucer.  

#### Downloading NEK
- You can access the NEK-v1.0.0-alpha prerelease in the Releases section. 
- Note that this VST3 release is for Windows platforms. 
- Hopefully, a release for MAC and Linux will be available soon.

## Contributing
If you want to contribute, please send me a pull request! If you want to also modify the code for your use, feel free to do so. A shoutout will be appreciated as well :-).

## Licensing
To the best of my knowledge, you can build this software for your own personal use. I am yet to fully understand the idiosyncrasies that come with Licensing with JUCE. But the software will be covered under a GPL License. Feel free to clone the repo and edit the code for your own personal use.
