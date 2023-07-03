/*
  ==============================================================================

    CircleBuffer.h
    Created: 27 Jun 2023 2:52:21pm
    Author:  kaarl

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
class CircleBuffer {
public:
    CircleBuffer();
    CircleBuffer(int bufferLength, int delay);
    void nextSample();
    float getData();
    void setData(float data);
private:
    juce::AudioSampleBuffer buffer;
    int readIndex;
    int writeIndex;
    int delay;
};