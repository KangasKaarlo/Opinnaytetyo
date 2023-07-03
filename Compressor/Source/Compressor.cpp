/*
  ==============================================================================

    Compressor.cpp
    Created: 27 Jun 2023 3:23:04pm
    Author:  kaarl

  ==============================================================================
*/

#pragma once

#include "Compressor.h"
#include <cmath>;
#include <algorithm>


Compressor::Compressor()
{
    buffer = CircleBuffer(400000, 0);
    tav = 0.01f;
    rms = 0;
    gain = 1;
}

float Compressor::compress(float data, float threshold, float ratio, float attack, float release, float knee)
{
    //Detecting the input gain and store it as a DB value
    rms = (1 - tav) * rms + tav * std::pow(data, 2.0f);
    float dbRMS = 10 * std::log10(rms); 

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
