/*
  ==============================================================================

    AmpAudioProcessor.cpp
    Created: 18 Apr 2025 6:14:04pm
    Author:  Marius

  ==============================================================================
*/

#include "Processor.h"

#include <filesystem>
#include <iostream>



SkeletonAudioProcessor::SkeletonAudioProcessor(juce::AudioProcessorValueTreeState& inParameters, ParameterSetup& inParameterSetup)
    : AudioProcessor(BusesProperties().withInput("Input", AudioChannelSet::stereo())
        .withOutput("Output", AudioChannelSet::stereo()))
    , mParameters(inParameters)
    , mParameterSetup(inParameterSetup)
    , mBlockSize(256)
    , mSampleRate(44100)
    , mToneStack(new dsp::tone_stack::BasicNamToneStack(mParameterSetup))
    , mParamListener(*this,mToneStack, mParameterSetup)

{

    setRateAndBufferSizeDetails(mSampleRate, mBlockSize);

    if (!mFloatBuffer)
    {
        mFloatBuffer = new float* [getNumInputChannels()];
        for (int channel = 0; channel < getNumInputChannels(); ++channel)
        {
            mFloatBuffer[channel] = new float[1024];
        }
    }
    if (!mTempFloatBuffer)
    {
        mTempFloatBuffer = new float* [getNumInputChannels()];
        for (int channel = 0; channel < getNumInputChannels(); ++channel)
        {
            mTempFloatBuffer[channel] = new float[1024];
        }
    }

    mParameters.addParameterListener("mid", &mParamListener);
    mToneStack->Reset(getSampleRate(), getBlockSize());

}
SkeletonAudioProcessor::~SkeletonAudioProcessor()
{
    for (int channel = 0; channel < getNumInputChannels(); ++channel)
    {
        delete[] mFloatBuffer[channel];
        delete[] mTempFloatBuffer[channel];
    }
    delete[] mFloatBuffer;
    delete[] mTempFloatBuffer;
}  
void SkeletonAudioProcessor::updateMeter(bool isOutput, AudioBuffer<float>& buffer,int numSamples,int numChannels)
{
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = buffer.getReadPointer(channel);
        float sum = 0.0f;

        for (int i = 0; i < numSamples; ++i)
        {
            sum += channelData[i] * channelData[i];
        }

        float rms = std::sqrt(sum / numSamples);

        if (channel == 0)
        {
            mRmsLevelLeft.store(rms);
            mRmsLevelRight.store(rms);
        }
    }
}
void SkeletonAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer&)
{

    int isMono = 1;

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    updateMeter(false, buffer, numSamples, isMono);
    

    if (!mFloatBuffer)
    {
        mFloatBuffer = new float* [isMono];
        mTempFloatBuffer = new float* [isMono];
        for (int channel = 0; channel < isMono; ++channel)
        {
            mFloatBuffer[channel] = new float[1024];
            mTempFloatBuffer[channel] = new float[1024];
        }
    }

    if (!mDoubleBuffer)
    {
        mDoubleBuffer = new double* [isMono];
        mTempDoubleBuffer = new double* [isMono];
        for (int channel = 0; channel < isMono; ++channel)
        {
            mDoubleBuffer[channel] = new double[1024];
            mTempDoubleBuffer[channel] = new double[1024];
        }
    }
    for (int channel = 0; channel < isMono; ++channel)
    {
        auto* floatData = buffer.getReadPointer(channel);

        std::copy(floatData, floatData + numSamples, mFloatBuffer[channel]);
    }


    for (int channel = 0; channel < isMono; ++channel)
    {
        std::transform(mFloatBuffer[channel], mFloatBuffer[channel] + numSamples, mDoubleBuffer[channel], [](float sample) {
            return static_cast<double>(sample);
            });
    }


    auto param = mParameters.getParameterAsValue("radiationAmplitude").getValue();
    auto param2 = mParameters.getParameterAsValue("radiationLength").getValue();
    auto param3 = mParameters.getParameterAsValue("radiationFrequency").getValue();

    for (int channel = 0; channel < isMono; ++channel)
    {
        std::transform(mDoubleBuffer[channel], mDoubleBuffer[channel] + numSamples, mTempFloatBuffer[channel], [](double sample) {
            return static_cast<float>(sample);
            });
    }
    for (int channel = 0; channel < isMono; ++channel)
    {
        auto* floatData = buffer.getWritePointer(channel);
        std::copy(mTempFloatBuffer[channel], mTempFloatBuffer[channel] + numSamples, floatData);
    }

    bool mMonoToStereo = true;
    if (mMonoToStereo && numChannels > 1)
    {
        auto* floatData = buffer.getWritePointer(1);
        auto* monoData = buffer.getReadPointer(0);
        std::copy(monoData, monoData + numSamples, floatData);
    }


    buffer.applyGain(mParameters.getParameterAsValue("output").getValue());

    updateMeter(true, buffer, numSamples, numChannels); 
}