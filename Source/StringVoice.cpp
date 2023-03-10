/*
  ==============================================================================

    StringVoice.cpp
    Created: 9 Mar 2023 9:58:16am
    Author:  Sonderbo

  ==============================================================================
*/

#include "StringVoice.h"

bool StringVoice::canPlaySound(juce::SynthesiserSound* sound)
{
	//Return true if sound is valid
	return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void StringVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
	
}

void StringVoice::stopNote(float velocity, bool allowTailOff)
{
	
}

void StringVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void StringVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels)
{

}

void StringVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{

}

void StringVoice::pitchWheelMoved(int newPitchWheelValue)
{

}