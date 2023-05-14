/*
  ==============================================================================

    Plate.h
    Created: 13 Apr 2023 1:00:42pm
    Author:  Sønderbo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Definitions.h"
//==============================================================================
/*
*/
class Plate
{
public:
    Plate();

    void prepareToPlay(double sampleRate);
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples);
    void setParameters(const juce::NamedValueSet& valueSet);

    void excite();
    float getDisplacement(int n, float ratioX, float ratioY);
    void updateStates();

    float geth();
    float getH();
    float getRho();
    float getSigma0();

    void applyConnectionForce(float f, float ratioX, float ratioY);
private:

    //Util
    double sampleRate;
    float k; // Sampling period

    float S;

    //Plate
    float h; //Grid spacing
    float hSq;
    int Ny; //Interval count for length
    int Nx; //Interval count for width
    float Lx = 1.5f; //Plate length
    float Ly = 1.0f; //Plate width
    float D; //Stiffness coefficient
    const float E = 2 * pow(10, 11); //Young's Modulus
    float H = 0.01; //Thickness
    float v = 0.3f; //Poisson's ratio
    float sigma_0 = 1.0f;//Frequency independent damping
    float sigma_1 = 0.005f;//Frequency dependent damping
    float rho = 7850.0f; //Material Density (kg/m^3)
    float kappa;//Stiffness coefficient
    float mu;
    float muSq;

    std::vector<std::vector<std::vector<float>>> uStates;
    std::vector<std::vector<float>*> u;

    // Methods ===================================================================
    void deriveParameters();
    float getOutput(float ratio);
    void calculateScheme();
    
    float limit(float sample);
};
