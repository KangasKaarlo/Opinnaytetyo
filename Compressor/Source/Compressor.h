/*
  ==============================================================================

    Compressor.h
    Created: 27 Jun 2023 3:23:04pm
    Author:  kaarl

  ==============================================================================
*/

#include "CircleBuffer.h"
#pragma once
class Compressor {
public:
    Compressor();
    float compress(float data, float treshold, float ratio, float attack, float release, float knee);
private:
    CircleBuffer buffer;
    float tav;
    float rms;
    float gain;
};