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
	excite();
}

void StringVoice::stopNote(float velocity, bool allowTailOff)
{

}

void StringVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void StringVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

void StringVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels)
{
	this->sampleRate = sampleRate;
	k = 1 / sampleRate;

	deriveParameters();

	excite();
}

void StringVoice::deriveParameters()
{
	h = c * k;
	N = (int)(L / h);
	h = (float)L / (float)N;
	lambdaSq = (c * c) * (k * k) / (h * h);
	hSq = h * h;
	kappa = sqrtf(E * I / rho * A);
	A = juce::MathConstants<float>::pi * (r * r);
	I = juce::MathConstants<float>::pi * pow(r, 4) / 4;
	mu = (kappa*k) / hSq;

	uStates = std::vector<std::vector<float>>(3, std::vector<float>(N + 1, 0));
	u.resize(3, nullptr);

	for (int i = 0; i < 3; i++)
	{
		u[i] = &uStates[i][0];
	}

	float stable = sqrtf((c * c * k * k + 4 * sigma_1 * k + sqrtf(pow(c * c * k * k + 4 * sigma_1 * k, 2) + 16 * kappa * kappa * k * k))/2);

	float _h = c * k;

	if (_h < stable)
	{
		DBG("UNSTABLE");
	}
}

void StringVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	//if (!isVoiceActive()) return;

	while (--numSamples >= 0)
	{
		float sample = 0.0f;
		calculateScheme();

		sample = limit(getOutput(0.3));

		for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
		{
			outputBuffer.addSample(channel, startSample, sample);
		}

		updateStates();
		startSample++;
	}
}

void StringVoice::excite()
{
	const float width = 10.0f;
	const float excitationLoc = 0.2f;

	int start = std::max(floor((N + 1) * excitationLoc) - floor(width * 0.5f), 1.0f);

	for (int l = 0; l < width; l++)
	{
		if (l + start > N - 1)
		{
			break;
		}

		u[1][l + start] += 0.5 * (1 - cos(2.0 * juce::MathConstants<float>::pi * l / (width - 1.0)));
		u[0][l + start] += 0.5 * (1 - cos(2.0 * juce::MathConstants<float>::pi * l / (width - 1.0)));

	}
}

void StringVoice::calculateScheme()
{
	//Why this?
	/*for (int i = 1; i < N; i++)
	{
		u[0][i] = 2.0 * u[1][i] - u[2][i] + lambdaSq * (u[1][i + 1] - 2.0 * u[1][i] + u[1][i - 1]);
	}*/

	//1D Wave equation - Update equation
	/*for (int l = 1; l < N; l++)
	{
		u[2][l] = (2 - 2 * lambdaSq) * u[1][l] + lambdaSq * (u[1][l + 1] + u[1][l - 1]) - u[0][l];
	}*/

	//Clamped boundary
	//Update equation for stiff string
	for (int l = 1; l < N; l++)
	{
		u[2][l] = ((2 - 2 * lambdaSq - 6*muSq - ((4 * sigma_1 * k) / hSq)) * u[1][l]
			+ (lambdaSq + 4 * muSq + ((2 * sigma_1 * k) / hSq)) * (u[1][l + 1] + u[1][l - 1])
			- muSq * (u[1][l + 2] + u[1][l - 2]) + (-1 + sigma_0 * k + ((4 * sigma_1 * k) / hSq)) * u[0][l]
			- (2 * sigma_1 * k) / hSq * (u[0][l + 1] + u[0][l - 1]))
			/(1+sigma_0*k);
	}
}

void StringVoice::updateStates()
{
	auto uPrev = u[0];
	u[0] = u[1];
	u[1] = u[2];
	u[2] = uPrev;
}

float StringVoice::getOutput(float ratio)
{
	return u[1][static_cast<int>(round(N * ratio))];
}

float StringVoice::limit(float sample)
{
	if (sample > 1.0f)
	{
		DBG("Sample Exceeded 1.0");
		return 1.0f;
	}	
	else if (sample < -1.0f)
	{
		DBG("Sample Exceeded -1.0");
		return -1.0f;
	}
	else
		return sample;
}