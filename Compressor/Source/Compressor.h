/*
  ==============================================================================

	Compressor.h
	Created: 27 Jun 2023 3:23:04pm
	Author:  kaarl

  ==============================================================================
*/
#pragma once
#include "EnvelopeDetector.h"
/*class Compressor {
public:
	Compressor();
	float compress(float data,float dbRMS ,float treshold, float ratio, float attack, float release, float knee);
	float calculteDbRMS(float data);
private:
	CircleBuffer buffer;
	float tav;
	float rms;
	float gain;
};*/
class Compressor {
public:
	float processAudioSample(float sample);
	void setAttack(float attack);
	void setRelease(float release);
	void setTreshold(float treshold);
	EnvelopeDetector envelopeDetector;

private:
	//internal
	float computeGain(float input);
	float threshold_dB ;
	float attackTime_ms;
	float releaseTime_ms;
	float ratio = 4.0f;
	float kneeWidth_dB;
	bool hardLimitGate = false;
	bool softKnee = true;
	bool sidechain = false;
	float outputGain_db = 0.0f;

	//outbound for GUI
	float gainReduction = 0.0f;
	float gainReduction_db = 0.0f;
};

