/*
  ==============================================================================

    AmpAudioProcessor.cpp
    Created: 18 Apr 2025 6:14:04pm
    Author:  Marius

  ==============================================================================
*/

#include "AmpAudioProcessor.h"

#include <filesystem>
#include <iostream>



AmpAudioProcessor::AmpAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::stereo())
    . withOutput("Output", AudioChannelSet::stereo()))
    , mParameters(*this, nullptr, "PARAMETERS", createParameterLayout())
    , mParameterSetup()
    , mSkeletonAmpProcessor(mParameters,mParameterSetup)
{

}
AmpAudioProcessor::~AmpAudioProcessor()
{
   
}

void AmpAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer&)
{
  mSkeletonAmpProcessor.processBlock(buffer, MidiBuffer());

}
