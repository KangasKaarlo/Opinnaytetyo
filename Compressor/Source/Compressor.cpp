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


/*
Compressor::Compressor()
{
    buffer = CircleBuffer(100000000, 0);
    tav = 0.01f;
    rms = 0;
    gain = 1;
}

float Compressor::compress(float data,float dbRMS, float threshold, float ratio, float attack, float release, float knee)
{
    //Calculates the new gain value
    float slope = 1 - (1 / ratio);
    float dbGain = std::min(0.0f, (slope * (threshold - dbRMS)));
    float newGain = std::pow(10, dbGain / 20);

    //Smoothen the transition with attack and release
    float coeff;
    if (newGain < gain) coeff = attack;
    else coeff = release;
    gain = (1 - coeff) * gain + coeff * newGain;

    //Update buffer
    float compressedSample = gain * buffer.getData();
    buffer.setData(data);
    buffer.nextSample();

    return compressedSample;
}

float Compressor::calculteDbRMS(float data)
{
    rms = (1 - tav) * rms + tav * std::pow(data, 2.0f);   
    return 10 * std::log10(rms);
}
*/

float Compressor::processAudioSample(float sample)
{
    float detectorOutput = envelopeDetector.processAudioSample(sample);
    double gainReduction = computeGain(detectorOutput);
    double makeupGain = pow(10.0, outputGain_db / 20.0);
    return sample * gainReduction /* makeupGain*/;
}

float Compressor::computeGain(float input) {
    float output = 0.0f;
    // --- below threshold, unity
    if (input <= threshold_dB)
        output = input;
    else// --- above threshold, compress
    {
        output = threshold_dB + (input - threshold_dB) / ratio;
    }
    return pow(10.0, (output - input) / 20.0);
}

void Compressor::setAttack(float attack)
{
    attackTime_ms = attack;
    envelopeDetector.setAttackTime(attack);
    DBG("in comp");
    DBG("attack");
    DBG(attackTime_ms);
    DBG("release");
    DBG(releaseTime_ms);
    DBG("treshold");
    DBG(threshold_dB);
}

void Compressor::setRelease(float release)
{
    releaseTime_ms = release;
    envelopeDetector.setReleaseTime(release);
    DBG("in comp");
    DBG("attack");
    DBG(attackTime_ms);
    DBG("release");
    DBG(releaseTime_ms);
    DBG("treshold");
    DBG(threshold_dB);
}

void Compressor::setTreshold(float treshold)
{
    threshold_dB = treshold;
    DBG("in comp");
    DBG("attack");
    DBG(attackTime_ms);
    DBG("release");
    DBG(releaseTime_ms);
    DBG("treshold");
    DBG(threshold_dB);
}


