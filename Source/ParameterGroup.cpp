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
ParameterGroup::ParameterGroup(HummelAudioProcessor& p, juce::String groupTitle, 
    std::vector<juce::String> ids, std::vector<juce::String> names)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    if (ids.size() != names.size())
    {
        jassert("Id count does not match names count for parameter group");
    }

    this->groupTitle = groupTitle;

    for (int i = 0; i < ids.size(); i++)
    {
        parameterKnobs.add(new ParameterKnob(p, ids[i], names[i]));
        addAndMakeVisible(parameterKnobs[i]);
    }
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

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText(groupTitle, 8.0f, 8.0f, 100.0f, 15.0f, juce::Justification::centred, 1);
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
