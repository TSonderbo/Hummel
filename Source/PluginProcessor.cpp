/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HummelAudioProcessor::HummelAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
	int voices = 3;
	for (int i = 0; i < voices; i++)
	{
		synth.addVoice(new StringVoice());
	}
	synth.addSound(new StringSound());
}

HummelAudioProcessor::~HummelAudioProcessor()
{
}

//==============================================================================
const juce::String HummelAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool HummelAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool HummelAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool HummelAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double HummelAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int HummelAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int HummelAudioProcessor::getCurrentProgram()
{
	return 0;
}

void HummelAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String HummelAudioProcessor::getProgramName(int index)
{
	return {};
}

void HummelAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void HummelAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	synth.setCurrentPlaybackSampleRate(sampleRate);

	for (int i = 0; i < synth.getNumVoices(); i++)
	{
		if (auto voice = dynamic_cast<StringVoice*>(synth.getVoice(i)))
		{
			voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels(),
				plate, 0.2f, 0.2f + (0.6f / synth.getNumVoices() * i));
		}
	}

	plate.prepareToPlay(sampleRate);
}

void HummelAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HummelAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void HummelAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	auto numSamples = buffer.getNumSamples();

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
	{
		buffer.clear(i, 0, numSamples);
	}

	checkParameterValues();

	for (int i = 0; i < numSamples; i++)
	{
		//Get plate wStar
		plate.renderNextBlock(buffer, i, 1);
		//Get string uStar, apply connection forces & read output into buffer
		synth.renderNextBlock(buffer, midiMessages, i, 1);
		// Update uStates for strings and plate
		plate.updateStates();
		for (int j = 0; j < synth.getNumVoices(); j++)
		{
			if (auto voice = dynamic_cast<StringVoice*>(synth.getVoice(j)))
			{
				voice->updateStates();
			}
		}
	}

	//This has proper midi behavior - the above does not...
	//synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
	//plate.renderNextBlock(buffer, 0, buffer.getNumSamples());

	scopeDataCollector.process(buffer.getReadPointer(0), (size_t)numSamples);
}

//==============================================================================
bool HummelAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HummelAudioProcessor::createEditor()
{
	return new HummelAudioProcessorEditor(*this);
}

//==============================================================================
void HummelAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void HummelAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new HummelAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout HummelAudioProcessor::createParams()
{
	std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

	//Inserts params here...

	//ADSR
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_String_Attack, NAME_String_Attack, juce::NormalisableRange<float>(0.1f, 1.0f), 0.1f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_String_Decay, NAME_String_Decay, juce::NormalisableRange<float>(0.1f, 1.0f), 0.1f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_String_Sustain, NAME_String_Sustain, juce::NormalisableRange<float>(0.1f, 1.0f), 0.9f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_String_Release, NAME_String_Release, juce::NormalisableRange<float>(0.1f, 1.0f), 0.4f));
	params.push_back(std::make_unique<juce::AudioParameterBool>(ID_String_ADSR_Toggle, NAME_String_ADSR_Toggle, false));

	//Add params for string
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_String_T, NAME_String_T, juce::NormalisableRange<float>(200.0f, 2000.0f), 1000.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_String_Rho, NAME_String_Rho, juce::NormalisableRange<float>(1000.0f, 16000.0f), 7850.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_String_Sigma_0, ID_String_Sigma_1, juce::NormalisableRange<float>(0.01f, 2.0f), 1.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_String_Sigma_1, NAME_String_Sigma_1, juce::NormalisableRange<float>(0.0f, 1.0f), 0.05f));

	//Add params for plate
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_Plate_H, NAME_Plate_H, juce::NormalisableRange<float>(0.001f, 0.05f), 0.01f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_Plate_Rho, NAME_Plate_Rho, juce::NormalisableRange<float>(350.0f, 10000.0f), 7850.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_Plate_Sigma_0, NAME_Plate_Sigma_0, juce::NormalisableRange<float>(0.01f, 2.0f), 1.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_Plate_Sigma_1, NAME_Plate_Sigma_1, juce::NormalisableRange<float>(0.0f, 1.0f), 0.05f));

	//Add params for connection
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_Connection_K1, NAME_Connection_K1, juce::NormalisableRange<float>(1000.0f, 20000.0f), 10000.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_Connection_K3, NAME_Connection_K3, juce::NormalisableRange<float>(1000000.0f, 20000000.0f), 10000000.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_Connection_R, NAME_Connection_R, juce::NormalisableRange<float>(0.01f, 2.0f), 1.0f));

	//Add params for excitation
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_String_Input_Amp, NAME_String_Input_Amp, juce::NormalisableRange<float>(0.1f, 1.0f), 0.5f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_String_Input_Width, NAME_String_Input_Width, juce::NormalisableRange<float>(1.0f, 20.0f), 10.0f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>(ID_String_Input_Loc, NAME_String_Input_Loc, juce::NormalisableRange<float>(0.1f, 0.9f), 0.5f));


	return { params.begin(), params.end() };
}

void HummelAudioProcessor::checkParameterValues()
{
	bool stringValueChanged = false;
	stringValueChanged |= valueSet.set(ID_String_T, apvts.getRawParameterValue(ID_String_T)->load());
	stringValueChanged |= valueSet.set(ID_String_Rho, apvts.getRawParameterValue(ID_String_Rho)->load());
	stringValueChanged |= valueSet.set(ID_String_Sigma_0, apvts.getRawParameterValue(ID_String_Sigma_0)->load());
	stringValueChanged |= valueSet.set(ID_String_Sigma_1, apvts.getRawParameterValue(ID_String_Sigma_1)->load());

	stringValueChanged |= valueSet.set(ID_String_Attack, apvts.getRawParameterValue(ID_String_Attack)->load());
	stringValueChanged |= valueSet.set(ID_String_Decay, apvts.getRawParameterValue(ID_String_Decay)->load());
	stringValueChanged |= valueSet.set(ID_String_Sustain, apvts.getRawParameterValue(ID_String_Sustain)->load());
	stringValueChanged |= valueSet.set(ID_String_Release, apvts.getRawParameterValue(ID_String_Release)->load());
	stringValueChanged |= valueSet.set(ID_String_ADSR_Toggle, apvts.getRawParameterValue(ID_String_ADSR_Toggle)->load());

	stringValueChanged |= valueSet.set(ID_Connection_K1, apvts.getRawParameterValue(ID_Connection_K1)->load());
	stringValueChanged |= valueSet.set(ID_Connection_K3, apvts.getRawParameterValue(ID_Connection_K3)->load());
	stringValueChanged |= valueSet.set(ID_Connection_R, apvts.getRawParameterValue(ID_Connection_R)->load());

	stringValueChanged |= valueSet.set(ID_String_Input_Amp, apvts.getRawParameterValue(ID_String_Input_Amp)->load());
	stringValueChanged |= valueSet.set(ID_String_Input_Width, apvts.getRawParameterValue(ID_String_Input_Width)->load());
	stringValueChanged |= valueSet.set(ID_String_Input_Loc, apvts.getRawParameterValue(ID_String_Input_Loc)->load());

	for (int i = 0; i < synth.getNumVoices(); i++)
	{
		if (auto voice = dynamic_cast<StringVoice*>(synth.getVoice(i)))
		{
			if (stringValueChanged)
			{
				voice->setParameters(valueSet);
			}
		}
	}

	bool plateValueChanged = false;
	plateValueChanged |= valueSet.set(ID_Plate_H, apvts.getRawParameterValue(ID_Plate_H)->load());
	plateValueChanged |= valueSet.set(ID_Plate_Rho, apvts.getRawParameterValue(ID_Plate_Rho)->load());
	plateValueChanged |= valueSet.set(ID_Plate_Sigma_0, apvts.getRawParameterValue(ID_Plate_Sigma_0)->load());
	plateValueChanged |= valueSet.set(ID_Plate_Sigma_1, apvts.getRawParameterValue(ID_Plate_Sigma_1)->load());

	if (plateValueChanged)
	{
		plate.setParameters(valueSet);
	}


}

AudioBufferQueue& HummelAudioProcessor::getAudioBufferQueue()
{
	return audioBufferQueue;
}

void HummelAudioProcessor::excitePlate()
{
	//TODO Needs mutex
	plate.excite();
}