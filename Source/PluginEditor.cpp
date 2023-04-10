/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HummelAudioProcessorEditor::HummelAudioProcessorEditor (HummelAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), oscilloscope(audioProcessor.getAudioBufferQueue())
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 400);

    addAndMakeVisible(parameterGroup);
    addAndMakeVisible(oscilloscope);
}

HummelAudioProcessorEditor::~HummelAudioProcessorEditor()
{
}

//==============================================================================
void HummelAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void HummelAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    //const float paddingX = 10;
    //const float paddingY = 10;

    //parameterGroup.setBounds(paddingX, paddingY, parameterGroup.getWidth(), parameterGroup.getHeight());

    //oscilloscope.setBounds(paddingX, parameterGroup.getBottom(), getWidth() - 2 * paddingX - parameterGroup.getWidth(), getHeight() - 2 * paddingY - parameterGroup.getHeight());

    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::column;

    fb.items.add(juce::FlexItem(parameterGroup)
        .withMinWidth(200.0f)
        .withMinHeight(100.0f)
        .withFlex(1.0f));

    fb.items.add(juce::FlexItem(oscilloscope)
        .withMinWidth(200.0f)
        .withMinHeight(200.0f)
        .withFlex(1.0f));

    fb.performLayout(getLocalBounds().toFloat());
}
