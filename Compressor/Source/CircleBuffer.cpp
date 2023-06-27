/*
  ==============================================================================

    CircleBuffer.cpp
    Created: 27 Jun 2023 2:52:21pm
    Author:  kaarl

  ==============================================================================
*/

#include "CircleBuffer.h"

CircleBuffer::CircleBuffer()
{
    buffer = juce::AudioSampleBuffer();
    writeIndex = 0;
    readIndex = 0;
    delay = 0;
}

CircleBuffer::CircleBuffer(int bufferLength, int delay)
{
    buffer = juce::AudioSampleBuffer(1, bufferLength);
    buffer.clear();
    writeIndex = delay;
    readIndex = 0;
    this->delay = delay;
}

void CircleBuffer::nextSample()
{
    int bufferLength = buffer.getNumSamples();
    readIndex = ((bufferLength + writeIndex) - delay % bufferLength);
    writeIndex = (writeIndex + 1) % bufferLength;
}

float CircleBuffer::getData()
{
    return buffer.getSample(0, readIndex);
}

void CircleBuffer::setData(float data)
{
    buffer.setSample(0, writeIndex, data);
}
