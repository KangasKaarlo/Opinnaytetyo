/*
  ==============================================================================

    EnvelopeDetector.cpp
    Created: 4 Jul 2023 3:38:59pm
    Author:  kaarl

  ==============================================================================
*/
#pragma once
#include "EnvelopeDetector.h"
#include <corecrt_math.h>
#include "JuceHeader.h"

float EnvelopeDetector::processAudioSample(float data)
{
    //Full-wave rectify
    float input = fabs(data);

    //Square it for RMS
    input *= input;

    //See if we're attacking or releasing
    float currentEnv = input;
    if (input > lastEnv) {
        currentEnv = attackTime * (lastEnv - input) + input;
    }
    else {
        currentEnv = releaseTime * (lastEnv - input) + input;
    }

    currentEnv = fmax(currentEnv, 0.0);
    lastEnv = currentEnv;

    //SQRT for RMS
    currentEnv = pow(currentEnv, 0.5);

    if (currentEnv <= 0) {
        return -96.0f;
    }
    //output in DB
    return 20.0f * log10(currentEnv);
}

bool EnvelopeDetector::reset()
{
    lastEnv = 0.0f;
    return true;
}

void EnvelopeDetector::setSampleRate(float sampleRate)
{
    this->sampleRate = sampleRate;
}

void EnvelopeDetector::setAttackTime(float attackTime)
{
    if (attackTime_ms != attackTime) {
        attackTime_ms = attackTime;
        this->attackTime = exp(TLD_AUDIO_ENVELOPE_ANALOG_TC / (attackTime_ms * sampleRate * 0.001));
    }
    DBG("in Env");
    DBG("attack");
    DBG(attackTime_ms);
    DBG("release");
    DBG(releaseTime_ms);
}

void EnvelopeDetector::setReleaseTime(float releaseTime)
{
    if (releaseTime_ms != releaseTime) {
        releaseTime_ms = releaseTime;
        this->releaseTime = exp(TLD_AUDIO_ENVELOPE_ANALOG_TC / (releaseTime_ms * sampleRate * 0.001));
    }
    DBG("in Env");
    DBG("attack");
    DBG(attackTime_ms);
    DBG("release");
    DBG(releaseTime_ms);
}
