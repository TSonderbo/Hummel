/*
  ==============================================================================

    ParameterGroup.h
    Created: 6 Apr 2023 9:57:42am
    Author:  Sønderbo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ParameterKnob.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class ParameterGroup  : public juce::Component
{
public:
    ParameterGroup(HummelAudioProcessor& p);
    ~ParameterGroup() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::OwnedArray<ParameterKnob> parameterKnobs;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterGroup)
};
