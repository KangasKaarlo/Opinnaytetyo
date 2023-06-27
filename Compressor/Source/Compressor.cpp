/*
  ==============================================================================

    Compressor.cpp
    Created: 27 Jun 2023 3:23:04pm
    Author:  kaarl

  ==============================================================================
*/

#include "Compressor.h"
#include <cmath>;
#include <algorithm>
#pragma once

Compressor::Compressor()
{
    buffer = CircleBuffer(150, 20);
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
    return 0.0f;
}
