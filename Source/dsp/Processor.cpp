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

    //// MAppers
    //auto gain = pSatBellGain->get();
    //auto freq = pSatBellFreq->get();
    //auto q = pSatBellQ->get();

    //const double sampleRate = getSampleRate();
    //const double midGainDB = gain; // +/- 10
    //const double midFrequency = freq;
    //const double midQuality = q;
    //recursive_linear_filter::BiquadParams midParams(sampleRate, midFrequency, midQuality, midGainDB);

    //mBellFilter.SetParams(midParams);

    // TO add in mappers

    int isMono = 2;

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

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

    // Bit Depth
	mParameters.getParameterAsValue("sampleRate").getValue();
	mParameters.getParameterAsValue("gainSat").getValue();
	mParameters.getParameterAsValue("satMix").getValue();

	auto pBitDepth = mParameters.getParameterAsValue("bitDepth");

	// Bit Depth
	const int sampleRate = getSampleRate();
	const int bitDepth = 16;  // Original bit depth)
    const int targetBitDepth = mParameters.getParameterAsValue("bitDepth").getValue();

    const int numLevels = 1 << targetBitDepth;   // Number of discrete levels (2^bitDepth)
    const double stepSize = 2.0 / numLevels;    // Step size between levels
    for (int channel = 0; channel < isMono; ++channel)
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            mTempDoubleBuffer[channel][sample] = std::round(mDoubleBuffer[channel][sample] / stepSize) * stepSize;
        }
    }

    // SampleRate

    for (int channel = 0; channel < isMono; ++channel)
    {
        float numOfSamplesForOneValue = getSampleRate() / int(mParameters.getParameterAsValue("sampleRate").getValue());
        int stepSize = static_cast<int>(numOfSamplesForOneValue); // Step size for downsampling

        for (int sample = 0; sample < numSamples; sample += stepSize)
        {
            // Take the first sample in the group (or apply a filter for better quality)
            double downsampledValue = mTempDoubleBuffer[channel][sample];

            // Fill the group with the downsampled value
            for (int s = 0; s < stepSize && (sample + s) < numSamples; ++s)
            {
                mDoubleBuffer[channel][sample + s] = downsampledValue;
            }
        }
    }

    // ADD the bell using AUdioDSPTools
    mTempDoubleBuffer = mBellFilter.Process(mDoubleBuffer, numChannels, numSamples);

    // Saturation 
    for (int channel = 0; channel < isMono; ++channel)
    {
        float inputGainDb = mParameters.getParameterAsValue("gainSat").getValue(); // Adjust this value as needed
        const double inputGainLinear = std::pow(10.0, inputGainDb / 20.0); // Convert dB to linear scale
        std::transform(mTempDoubleBuffer[channel], mTempDoubleBuffer[channel] + numSamples, mDoubleBuffer[channel],
            [inputGainLinear](double sample) {
                sample *= inputGainLinear;
                // Smooth buffy in lows 
//                // sample = std::tanh(sample);
//                // sample = std::atan(sample); // Arctangent saturation
//                // sample = std::sin(sample); // Sine wave saturation
//                // sample = sample - (1.5 * std::pow(sample, 3)); // Polynomial saturation
//                // sample = sample / (1.0 + std::abs(sample * 2.0)); // Wave shaping
//        // 
//                // HARD CLIP -> way more agressive
                {
                    const double threshold = 0.8; // Clipping threshold
                    if (sample > threshold) sample = threshold;
                    else if (sample < -threshold) sample = -threshold;
                }
                sample /= inputGainLinear;

                return sample;
            });
    }

    // Sat mix
    float mix = mParameters.getParameterAsValue("mix").getValue();
    for (int channel = 0; channel < isMono; channel++)
    {
        for (int s = 0; s < numSamples; s++)
        {
            mDoubleBuffer[channel][s] = (mDoubleBuffer[channel][s] * (mix)+mTempDoubleBuffer[channel][s] * (100 - mix)) * 0.01; // Carefull who get the mix
        }
    }

    /*
    for (int s = 0; s < numSamples; ++s)
    {
        if (nextImpulseSample <= 0)
        {
            // Génération d'un clic bruité sur plusieurs samples
            for (int k = 0; k < mGeigerLengthSample; ++k)
            {
                int sampleIndex = s + k;
                if (sampleIndex < numSamples)
                {
                    // Générer un bruit blanc léger UNE SEULE FOIS
                    float noise = mDistribution_noise(rng); // distribution entre -1.0f et 1.0f
                    float envelope = std::exp(-3.0f * static_cast<float>(k) / static_cast<float>(mGeigerLengthSample));
                    float impulseValue = noise * envelope * pRadiationAmplitude->get();

                    // Copier le même bruit dans tous les channels
                    for (int channel = 0; channel < numChannels; ++channel)
                    {
                        mTempDoubleBuffer[channel][sampleIndex] += impulseValue;
                    }
                }
            }

            // Préparer le prochain événement
            float secondsToNext = dist(rng);
            nextImpulseSample = static_cast<int>(secondsToNext * sampleRate);
        }
        else
        {
            nextImpulseSample--;
        }
    } */

    for (int channel = 0; channel < isMono; ++channel)
    {
        std::transform(mTempDoubleBuffer[channel], mTempDoubleBuffer[channel] + numSamples, mTempFloatBuffer[channel], [](double sample) {
            return static_cast<float>(sample);
            });
    }

    for (int channel = 0; channel < isMono; ++channel)
    {
        auto* floatData = buffer.getWritePointer(channel);
        std::copy(mTempFloatBuffer[channel], mTempFloatBuffer[channel] + numSamples, floatData);
    }

    // TRYYYYY
  
 //   updateMeter(false, buffer, numSamples, isMono);
    

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