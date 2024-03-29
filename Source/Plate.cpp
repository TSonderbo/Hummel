/*
  ==============================================================================

	Plate.cpp
	Created: 13 Apr 2023 1:00:42pm
	Author:  Sønderbo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Plate.h"

//==============================================================================
Plate::Plate()
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.

}

void Plate::prepareToPlay(double sampleRate)
{
	this->sampleRate = sampleRate;
	k = 1 / sampleRate;

	deriveParameters();
}

void Plate::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	while (--numSamples >= 0)
	{
		float sample = outputBuffer.getSample(0, startSample);

		calculateScheme();

		//sample = limit(getOutput(0.5));

		for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
		{
			//outputBuffer.addSample(channel, startSample, sample);
		}

		//updateStates();
		startSample++;
	}
}

void Plate::setParameters(const juce::NamedValueSet& valueSet)
{
	//Param update
	H = valueSet[ID_Plate_H];
	rho = valueSet[ID_Plate_Rho];
	sigma_0 = valueSet[ID_Plate_Sigma_0];
	sigma_1 = valueSet[ID_Plate_Sigma_1];

	//derive other parameters..
	deriveParameters();
}

void Plate::excite()
{
	float width = 10.0f;
	const float excitationLoc = 0.5f;
	
	int startx = std::max(floor((Nx + 1) * excitationLoc) - floor(width * 0.5f), 2.0f);
	int starty = std::max(floor((Ny + 1) * excitationLoc) - floor(width * 0.5f), 2.0f);

	for (int l = 0; l < width; l++)
	{
		juce::String str("");
		for (int m = 0; m < width; m++)
		{
			if (l + startx > Nx - 2 || m + starty > Ny - 2)
			{
				break;
			}
			
			u[1][l + startx][m + starty] += 0.1f * (0.5f * (1 - cos(juce::MathConstants<float>::twoPi * l / (width - 1.0)))) * ( 0.5f * (1 - cos(juce::MathConstants<float>::twoPi * m / (width - 1.0))));
			u[0][l + startx][m + starty] += 0.1f * (0.5f * (1 - cos(juce::MathConstants<float>::twoPi * l / (width - 1.0)))) * ( 0.5f * (1 - cos(juce::MathConstants<float>::twoPi * m / (width - 1.0))));
			
			std::string s = " ";
			s = s.append(std::to_string(u[1][l + startx][m + starty]));
			str.append(s, 6);
		}
		DBG(str);
	}

}

float Plate::getDisplacement(int n, float ratioX, float ratioY)
{
	int x, y;

	x = floor(ratioX * Nx);
	y = floor(ratioY * Ny);

	return u[n][x][y];
}

void Plate::deriveParameters()
{

	D = E * (H * H * H) / (12.0f * (1.0f - (v * v)));
	kappa = sqrtf(D / (rho * H));

	h = 2.0f * sqrtf(k * (sigma_1 + sqrtf(kappa * kappa + sigma_1 * sigma_1)));
	hSq = h * h;

	Nx = static_cast<int>(Lx / h);
	Ny = static_cast<int>(Ly / h);
	
	h = fmin(Lx / Nx, Ly / Ny);
	
	hSq = h * h;

	mu = (kappa * k) / hSq;
	muSq = mu * mu;

	S = 2.0f * sigma_1 * k / hSq;

	uStates = std::vector<std::vector<std::vector<float>>>(3, std::vector<std::vector<float>>(Nx, std::vector<float>(Ny, 0)));
	u.resize(3, nullptr);

	for (int i = 0; i < 3; i++)
	{
		u[i] = &uStates[i][0];
	}

	std::string str = "Plate gridpoints: ";
	str.append(std::to_string(Nx * Ny));

	DBG(str);
}

float Plate::getOutput(float ratio)
{
	return u[1][static_cast<int>(round(Nx * ratio))][static_cast<int>(round(Ny * ratio))];
}

void Plate::calculateScheme()
{
	//2D system
	/*for (int l = 2; l < Nx - 1; l++)
	{
		for (int m = 2; m < Ny - 1; m++)
		{
			u[2][l][m] = 2 * u[1][l][m] - u[0][l][m] + lambdaSq
				* (u[1][l + 1][m] + u[1][l - 1][m] + u[1][l][m + 1]
					+ u[1][l][m - 1] - 4 * u[1][l][m]);
		}
	}*/

	for (int l = 2; l < Nx - 2; l++)
	{
		for (int m = 2; m < Ny - 2; m++)
		{
			u[2][l][m] = ((2 - 20 * muSq - 4 * S) * u[1][l][m]
				+ (8 * muSq + S) * (u[1][l + 1][m] + u[1][l - 1][m] + u[1][l][m + 1] + u[1][l][m - 1])
				- 2 * muSq * (u[1][l + 1][m + 1] + u[1][l - 1][m + 1] + u[1][l + 1][m - 1] + u[1][l - 1][m - 1])
				- muSq * (u[1][l + 2][m] + u[1][l - 2][m] + u[1][l][m + 2] + u[1][l][m - 2])
				+ (sigma_0 * k - 1 + 4 * S) * u[0][l][m]
				- S * (u[0][l + 1][m] + u[0][l - 1][m] + u[0][l][m + 1] + u[0][l][m - 1]))
				/ (1 + sigma_0 * k);
		}
	}
}

void Plate::updateStates()
{
	auto uPrev = u[0];
	u[0] = u[1];
	u[1] = u[2];
	u[2] = uPrev;
}

float Plate::geth()
{
	return h;
}

float Plate::getH()
{
	return H;
}

float Plate::getRho()
{
	return rho;
}

float Plate::getSigma0()
{
	return sigma_0;
}

void Plate::applyConnectionForce(float f, float ratioX, float ratioY)
{
	int x, y;

	x = floor(ratioX * Nx);
	y = floor(ratioY * Ny);

	u[2][x][y] = u[2][x][y] + 1 / (h * h) * ((f * (k * k)) / (rho * H * (1 + sigma_0 * k)));
}

float Plate::limit(float sample)
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

