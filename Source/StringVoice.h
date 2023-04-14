/*
  ==============================================================================

    StringVoice.h
    Created: 9 Mar 2023 9:58:16am
    Author:  Sonderbo

  ==============================================================================
*/

#include <JuceHeader.h>

class StringVoice : public juce::SynthesiserVoice
{
public:

	//========================================================
#pragma region overrides
	bool canPlaySound(juce::SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void controllerMoved(int controllerNumber, int newControllerValue) override;
	void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
	void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
	void pitchWheelMoved(int newPitchWheelValue) override;
#pragma endregion
	//========================================================
	void setParameters(const juce::NamedValueSet& valueSet);
private:
	//Util
	double sampleRate;
	float k; // Sampling period

	//String
	float c = 400.0f; //Wavespeed
	float L = 1.0f; //Length of string
	float h; //Grid spacing
	float hSq; //Grid spacing squared
	int N; //Interval count
	float lambdaSq;//Courant number squared
	float rho = 7850.0f; //Material Density (kg/m^3)
	float r = 5 * pow(10, -4);//Radius
	float T = 1000.0f;//Tension
	const float E = 2 * pow(10, 11);//Young's modulus
	float sigma_0 = 1.0f;//Frequency independent damping
	float sigma_1 = 0.05f;//Frequency dependent damping
	float A; //Cross-sectional Area
	float kappa;//Stiffness coefficient
	float I; //Inertia
	float mu;
	float muSq;
	
	//State
	std::vector<std::vector<float>> uStates; //3 by N State matrix for wave equation
	std::vector<float*> u; //uStates pointer

	//Effects
	juce::ADSR adsr;


	void deriveParameters();
	void excite();
	float getOutput(float ratio);
	void calculateScheme();
	void updateStates();
	float limit(float sample);
};