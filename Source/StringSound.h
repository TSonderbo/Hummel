/*
  ==============================================================================

    StringSound.h
    Created: 9 Mar 2023 9:58:02am
    Author:  Sonderbo

  ==============================================================================
*/

#include <JuceHeader.h>

class StringSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
private:
};
