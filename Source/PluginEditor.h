/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ScopeComponent.h"
#include "ParameterGroup.h"

//==============================================================================
/**
*/
class HummelAudioProcessorEditor  : public juce::AudioProcessorEditor,
    juce::Button::Listener
    
{
public:
    HummelAudioProcessorEditor (HummelAudioProcessor&);
    ~HummelAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HummelAudioProcessor& audioProcessor;
    juce::TextButton exciteButton;
    ParameterGroup parameterGroup { audioProcessor };
    ScopeComponent oscilloscope;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HummelAudioProcessorEditor)
};
