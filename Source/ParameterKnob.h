/*
  ==============================================================================

    ParameterKnob.h
    Created: 6 Apr 2023 9:57:14am
    Author:  Sønderbo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
//==============================================================================
/*
*/
class ParameterKnob  : public juce::Component
{
public:
    ParameterKnob(HummelAudioProcessor& p, juce::String identifier);
    ~ParameterKnob() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider knob;
    juce::Label knobLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;

    HummelAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterKnob)
};
