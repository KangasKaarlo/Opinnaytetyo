/*
  ==============================================================================

    EnvelopeDetector.h
    Created: 4 Jul 2023 3:38:59pm
    Author:  kaarl

  ==============================================================================
*/

#pragma once

class EnvelopeDetector {
public:
    float processAudioSample(float data);
    bool reset();
    void setSampleRate(float sampleRate);
    void setAttackTime(float attackTime);
    void setReleaseTime(float releaseTime);
private:
    float attackTime;
    float releaseTime;
    float attackTime_ms;
    float releaseTime_ms;
    float lastEnv;
    float sampleRate;
    bool RMS = false;
    //Analogue time constant for a RC filter
    //log(36.7)
    const float TLD_AUDIO_ENVELOPE_ANALOG_TC = -0.99967234081320612357829304641019f;
};