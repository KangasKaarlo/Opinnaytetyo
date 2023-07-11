/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorAudioProcessor::CompressorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "parameters", createParameterLayout())
#endif
{
    treeState.addParameterListener("gain", this);
    treeState.addParameterListener("treshold", this);
    treeState.addParameterListener("attack", this);
    treeState.addParameterListener("release", this);
    treeState.addParameterListener("limit", this);
    treeState.addParameterListener("sidechain", this); 
}

CompressorAudioProcessor::~CompressorAudioProcessor()
{
    treeState.removeParameterListener("gain", this);
    treeState.removeParameterListener("treshold", this);
    treeState.removeParameterListener("attack", this);
    treeState.removeParameterListener("release", this);
    treeState.removeParameterListener("limit", this);
    treeState.removeParameterListener("sidechain", this);
}

//==============================================================================
const juce::String CompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

juce::AudioProcessorValueTreeState::ParameterLayout CompressorAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

    juce::NormalisableRange<float> attackRange(1.0f, 200.0f, 1);
    attackRange.setSkewForCentre(50.0f);

    juce::NormalisableRange<float> releaseRange(1.0f, 200.0f, 1);
    releaseRange.setSkewForCentre(80.0f);

    auto pGain = std::make_unique<juce::AudioParameterFloat>("gain", "Gain", -12.0, 12.0, 0.0);
    auto pTresh = std::make_unique<juce::AudioParameterFloat>("treshold", "Treshold", -60.0f, 10.0f, 0.0f);
    auto pAttack = std::make_unique<juce::AudioParameterFloat>("attack", "Attack", attackRange, 30);
    auto pRelease = std::make_unique<juce::AudioParameterFloat>("release", "Release", releaseRange, 100.0);
    auto pLimiting = std::make_unique<juce::AudioParameterBool>("limit", "Limiter", false);
    auto pSidechain = std::make_unique<juce::AudioParameterBool>("sidechain", "Sidechain", false);
    
    params.push_back(std::move(pGain));
    params.push_back(std::move(pTresh));
    params.push_back(std::move(pAttack));
    params.push_back(std::move(pRelease));
    params.push_back(std::move(pLimiting));
    params.push_back(std::move(pSidechain));
    return{ params.begin(), params.end() };
}

void CompressorAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    DBG(parameterID);
    DBG(newValue);
    if (parameterID == "gain") { 
        LComp.setGain(newValue);
    }
    else if (parameterID == "treshold") {
        LComp.setTreshold(newValue);
    }
    else if (parameterID == "attack") {
        LComp.setAttack(newValue);
    }
    else if (parameterID == "release") {
        LComp.setRelease(newValue);
    }
    else if (parameterID == "limit") {

    }
    else if (parameterID == "sidechain") {

    }
}


bool CompressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CompressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CompressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CompressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CompressorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CompressorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CompressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void CompressorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    /*for (int channel = 0; channel < getNumOutputChannels(); channel++) {
        allCompressors.add(Compressor());
    }*/
    LComp.envelopeDetector.setSampleRate(sampleRate);
}

void CompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CompressorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    if (totalNumInputChannels >= 2) {
        for (int i = 0; i < buffer.getNumSamples(); i++) {
            float sampleForComp = 0.0f;
            for (int channel = 0; channel < 2; ++channel)
            {
                auto* data = buffer.getWritePointer(channel);
                sampleForComp += data[i];
            }
            gain = LComp.returnGainMultiplier(sampleForComp/2);
            for (int channel = 0; channel < 2; ++channel)
            {
                auto* data = buffer.getWritePointer(channel);
                data[i] = data[i] * gain;
            }
        }
    }
}

//==============================================================================
bool CompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CompressorAudioProcessor::createEditor()
{
    //return new CompressorAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void CompressorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CompressorAudioProcessor();
}
