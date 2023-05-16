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
    exciteButton.setButtonText("Excite Plate");
    exciteButton.addListener(this);

    addParameterGroups();

    addAndMakeVisible(exciteButton);
    addAndMakeVisible(oscilloscope);

    //TODO Add toggle for ADSR

    setSize(1280, 720);
}

HummelAudioProcessorEditor::~HummelAudioProcessorEditor()
{
}

//==============================================================================
void HummelAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void HummelAudioProcessorEditor::buttonClicked(juce::Button* button)// [2]
{
    if(button == &exciteButton)
        audioProcessor.excitePlate();
}

void HummelAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    //const float paddingX = 10;
    //const float paddingY = 10;

    //parameterGroup.setBounds(paddingX, paddingY, parameterGroup.getWidth(), parameterGroup.getHeight());

    //oscilloscope.setBounds(paddingX, parameterGroup.getBottom(), getWidth() - 2 * paddingX - parameterGroup.getWidth(), getHeight() - 2 * paddingY - parameterGroup.getHeight());

    juce::FlexBox main;
    main.flexDirection = juce::FlexBox::Direction::row;

    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::column;

    juce::FlexBox paramBox;
    paramBox.flexDirection = juce::FlexBox::Direction::column;
    for(auto group : parameterGroups)
    {
        paramBox.items.add(juce::FlexItem(*group)
            .withMinWidth(250.0f)
            .withMinHeight(100.0f)
            .withFlex(1.0f));
    }

    fb.items.add(juce::FlexItem(oscilloscope)
        .withMinWidth(200.0f)
        .withMinHeight(200.0f)
        .withFlex(1.0f));

    fb.items.add(juce::FlexItem(exciteButton)
        .withMinWidth(50.0f)
        .withMinHeight(20.0f)
        .withMaxHeight(50.0f)
        .withFlex(2.0f));
    
    main.items.add(juce::FlexItem(fb)
        .withMinWidth(400.0f)
        .withMinHeight(720.0f)
        .withFlex(2.0f));

    main.items.add(juce::FlexItem(paramBox)
        .withMinWidth(600.0f)
        .withMinHeight(720.0f));

    main.performLayout(getLocalBounds().toFloat());
}

void HummelAudioProcessorEditor::addParameterGroups()
{
    //ADSR param group
    std::vector<juce::String> connectionIds{ ID_Connection_K1, ID_Connection_K3, ID_Connection_R};
    std::vector<juce::String> connectionNames{ NAME_Connection_K1, NAME_Connection_K3, NAME_Connection_R};
    parameterGroups.add(new ParameterGroup(audioProcessor, "Connection Parameters", connectionIds, connectionNames));
    //String param group
    std::vector<juce::String> stringIds{ ID_String_T, ID_String_Rho, ID_String_Sigma_0, ID_String_Sigma_1 };
    std::vector<juce::String> stringNames{ NAME_String_T, NAME_String_Rho, NAME_String_Sigma_0, NAME_String_Sigma_1 };
    parameterGroups.add(new ParameterGroup(audioProcessor, "String Parameters", stringIds, stringNames));
    //String input param group
    std::vector<juce::String> stringInputIds{ ID_String_Input_Amp, ID_String_Input_Width, ID_String_Input_Loc };
    std::vector<juce::String> stringInputNames{ NAME_String_Input_Amp, NAME_String_Input_Width, NAME_String_Input_Loc };
    parameterGroups.add(new ParameterGroup(audioProcessor, "String Input Parameters", stringInputIds, stringInputNames));
    //Plate param group
    std::vector<juce::String> plateIds{ ID_Plate_H, ID_Plate_Rho, ID_Plate_Sigma_0, ID_Plate_Sigma_1 };
    std::vector<juce::String> plateNames{ NAME_Plate_H, NAME_Plate_Rho, NAME_Plate_Sigma_0, NAME_Plate_Sigma_1 };
    parameterGroups.add(new ParameterGroup(audioProcessor, "Plate Parameters", plateIds, plateNames));

    for (auto group : parameterGroups)
    {
        addAndMakeVisible(*group);
    }
}