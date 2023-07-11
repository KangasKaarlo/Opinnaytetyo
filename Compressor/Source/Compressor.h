/*
  ==============================================================================

	Compressor.h
	Created: 27 Jun 2023 3:23:04pm
	Author:  kaarl

  ==============================================================================
*/
#pragma once
#include "EnvelopeDetector.h"

class Compressor {
public:
	float returnGainMultiplier(float sample);
	float processAudioSample(float sample);
	void setAttack(float attack);
	void setRelease(float release);
	void setTreshold(float treshold);
	void setGain(float gain);
	EnvelopeDetector envelopeDetector;

private:
	//internal
	float computeGain(float input);
	float threshold_dB ;
	float attackTime_ms;
	float releaseTime_ms;
	float ratio = 4.0f;
	float kneeWidth_dB = 0.5f;
	bool hardLimitGate = false;
	bool softKnee = true;
	bool sidechain = false;
	float outputGain_db = 0.0f;

	//outbound for GUI
	float gainReduction = 0.0f;
	float gainReduction_db = 0.0f;
};

