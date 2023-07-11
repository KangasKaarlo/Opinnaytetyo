/*
  ==============================================================================

	Compressor.cpp
	Created: 27 Jun 2023 3:23:04pm
	Author:  kaarl

  ==============================================================================
*/

#pragma once

#include "Compressor.h"
#include "JuceHeader.h"
#include <cmath>;
#include <algorithm>


float Compressor::returnGainMultiplier(float sample)
{
	float detectorOutput = envelopeDetector.processAudioSample(sample);
	double gainReduction = computeGain(detectorOutput);
	double makeupGain = pow(10.0, outputGain_db / 20.0);
	return gainReduction * makeupGain;
}

float Compressor::processAudioSample(float sample)
{
	float detectorOutput = envelopeDetector.processAudioSample(sample);
	double gainReduction = computeGain(detectorOutput);
	double makeupGain = pow(10.0, outputGain_db / 20.0);
	return sample * gainReduction * makeupGain;
}

float Compressor::computeGain(float input) {
	float output = 0.0f;

	if (softKnee) {
		//left, outside knee
		if (2.0 * (input - threshold_dB) < -kneeWidth_dB)
			output = input;
		//inside knee
		else if (2.0 * (fabs(input - threshold_dB)) <= kneeWidth_dB)
		{
			output = input + (((1.0 / ratio) - 1.0) * pow((input - threshold_dB + (kneeWidth_dB / 2.0)), 2.0)) / (2.0 * kneeWidth_dB);
		}
		//right, outside knee
		else if (2.0 * (input - threshold_dB) > kneeWidth_dB) {
			output = threshold_dB + (input - threshold_dB) / ratio;
		}
	}
	//HardKnee
	//below threshold, unity
	else if (input <= threshold_dB) {
		output = input;
	}
	else//above threshold, compress
	{
		output = threshold_dB + (input - threshold_dB) / ratio;
	}
	return pow(10.0, (output - input) / 20.0);
}

void Compressor::setAttack(float attack)
{
	attackTime_ms = attack;
	envelopeDetector.setAttackTime(attack);
}

void Compressor::setRelease(float release)
{
	releaseTime_ms = release;
	envelopeDetector.setReleaseTime(release);
}

void Compressor::setTreshold(float treshold)
{
	threshold_dB = treshold;
}

void Compressor::setGain(float gain)
{
	outputGain_db = gain;
}


