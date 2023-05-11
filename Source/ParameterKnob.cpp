/*
  ==============================================================================

    ParameterKnob.cpp
    Created: 6 Apr 2023 9:57:14am
    Author:  Sønderbo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ParameterKnob.h"

//==============================================================================
ParameterKnob::ParameterKnob(HummelAudioProcessor& p,juce::String identifier, juce::String name) :
processor(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    setSize(60, 110);

    knob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 25);
    
    knobLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    knobLabel.setFont(15.0f);
    knobLabel.setJustificationType(juce::Justification::centred);
    knobLabel.setText(name, juce::dontSendNotification);

    attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, identifier, knob));

    addAndMakeVisible(knobLabel);
    addAndMakeVisible(knob);
}

ParameterKnob::~ParameterKnob()
{
    
}

void ParameterKnob::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void ParameterKnob::resized()
{    
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::column;
    
    fb.items.add(juce::FlexItem(knobLabel)
        .withMinWidth(50.0f)
        .withMinHeight(10.0f)
        .withFlex(1));

    fb.items.add(juce::FlexItem(knob)
        .withMinWidth(50.0f)
        .withMinHeight(50.0f)
        .withFlex(1));

    fb.performLayout(getLocalBounds().toFloat());
}