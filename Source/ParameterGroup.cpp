/*
  ==============================================================================

    ParameterGroup.cpp
    Created: 6 Apr 2023 9:57:42am
    Author:  Sønderbo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ParameterGroup.h"

//==============================================================================
ParameterGroup::ParameterGroup(HummelAudioProcessor& p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    parameterKnobs.add(new ParameterKnob(p, "T" ));
    parameterKnobs.add(new ParameterKnob(p, "rho"));
    parameterKnobs.add(new ParameterKnob(p, "sigma0"));
    parameterKnobs.add(new ParameterKnob(p, "sigma1"));

    for (int i = 0; i < parameterKnobs.size(); i++)
        addAndMakeVisible(parameterKnobs[i]);
}

ParameterGroup::~ParameterGroup()
{
}

void ParameterGroup::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);

    auto bounds = getLocalBounds();
    bounds.reduce(4.0f, 4.0f);

    g.drawRect (bounds, 2);   // draw an outline around the component
}

void ParameterGroup::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    //const auto paddingX = 5;
    //const auto paddingY = 5;

    //for (int i = 0; i < parameterKnobs.size(); i++)
    //{
    //    if (i == 0) //First knob
    //    {
    //        parameterKnobs[i]->setBounds(paddingX, paddingY, parameterKnobs[i]->getWidth(), parameterKnobs[i]->getHeight());
    //    }
    //    else //Remaining knobs
    //    {
    //        parameterKnobs[i]->setBounds(parameterKnobs[i-1]->getRight(), paddingY, parameterKnobs[i]->getWidth(), parameterKnobs[i]->getHeight());
    //    }
    //}
        
    juce::FlexBox fb;

    fb.flexWrap = juce::FlexBox::Wrap::noWrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    fb.alignContent = juce::FlexBox::AlignContent::center;
    fb.alignItems = juce::FlexBox::AlignItems::center;

    for (auto* knob : parameterKnobs)
    {
        fb.items.add(juce::FlexItem(*knob)
            .withMinWidth(50.0f)
            .withMinHeight(100.0f)
            .withFlex(1));
    }
    
    auto rect = getLocalBounds();
    rect.reduce(8,8);
    fb.performLayout(rect);
}
