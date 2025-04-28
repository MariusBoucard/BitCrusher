/*
  ==============================================================================

   This file is part of the JUCE framework examples.
   Copyright (c) Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   to use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
   REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
   AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
   INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
   LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
   PERFORMANCE OF THIS SOFTWARE.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             GainPlugin
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Gain audio plugin.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_plugin_client, juce_audio_processors,
                   juce_audio_utils, juce_core, juce_data_structures,
                   juce_events, juce_graphics, juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2022

 moduleFlags:      JUCE_STRICT_REFCOUNTEDPOINTER=1

 type:             AudioProcessor
 mainClass:        GainProcessor

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/

#pragma once
#include <JuceHeader.h>
#include "dsp/RecursiveLinearFilter.h"
#include <random>
//==============================================================================
class GainProcessor final : public AudioProcessor, public AudioProcessorParameter::Listener
{
public:

    //==============================================================================
    GainProcessor()
        : AudioProcessor (BusesProperties().withInput  ("Input",  AudioChannelSet::stereo())
                                           .withOutput ("Output", AudioChannelSet::stereo()))
        , mDistribution_noise(-1.0f, 1.0f)
        , dist(5.f)
    {


        addParameter (gain = new AudioParameterFloat ({ "gain", 1 }, "Gain", 0.0f, 1.0f, 0.5f));
        addParameter(pBitDepth = new AudioParameterInt("bitDepth", "bitDepth", 1,16,16));
        addParameter(pSampleRate = new AudioParameterInt("sampleRate", "SampleRate", 1, 96000, 44100));

        addParameter(pSampleRateAlgoChoice = new AudioParameterChoice("satAlgo", "Saturation Algorithm", { "Soft Clip", "Hard Clip", "Wave shaping", "Polynomial saturation", "Tanh", "ArctanH"}, 1));
        addParameter(pGainSat = new AudioParameterFloat("gainSat", "Saturation gain", -12, 12, 0));
        addParameter(pSatMix = new AudioParameterFloat("mixSat", "Saturation mix", 0.f, 100.f, 100.f));

        addParameter(pSatBellFreq = new AudioParameterFloat("satBellFreq", "Sat Boost Freq", 10.f, 18000.f, 1000.f));
        addParameter(pSatBellGain = new AudioParameterFloat("satBellGain", "Sat Boost Gain", -12.f, 12.f, 0.f));
        addParameter(pSatBellQ = new AudioParameterFloat("satBellQ", "Sat Bell Queue", 0.f, 7.f, 3.f));

        addParameter(pMix = new AudioParameterFloat("mix", "Mix", 0.f, 100.f, 100.f));

        addParameter(pRadiationAmplitude = new AudioParameterFloat("radiationAmplitude", "Radiation Amplitude", 0.f, 1.f, 0.5f));
        addParameter(pRadiationsFrequence = new AudioParameterFloat("radiationFrequency", "Radiation Frequency", 0.1f, 50.f, 0.5f));
        addParameter(pRadiationLength = new AudioParameterFloat("radiationLength", "Radiation Length", 0.f, 1.f, 0.5f));
        pRadiationsFrequence->addListener(this);
        pRadiationLength->addListener(this);
        pRadiationAmplitude->addListener(this);


    }

    ~GainProcessor() override
    {
        for (int channel = 0; channel < 2; ++channel)
        {
			delete[] mFloatBuffer[channel];
			delete[] mTempFloatBuffer[channel];
			delete[] mDoubleBuffer[channel];
			delete[] mTempDoubleBuffer[channel];
			delete[] mVerbDoubleBuffer[channel];
		}
		delete[] mFloatBuffer;
		delete[] mTempFloatBuffer;
		delete[] mDoubleBuffer;
		delete[] mTempDoubleBuffer;
		delete[] mVerbDoubleBuffer;
        pRadiationsFrequence->removeListener(this);
        pRadiationLength->removeListener(this);
        pRadiationAmplitude->removeListener(this);
    }

    //==============================================================================
    void prepareToPlay (double, int) override {


    }

    void releaseResources() override {}

    void processBlock (AudioBuffer<float>& buffer, MidiBuffer&) override
    {

        // MAppers
        auto gain = pSatBellGain->get();
        auto freq = pSatBellFreq->get();
        auto q = pSatBellQ->get();

        const double sampleRate = getSampleRate();
        const double midGainDB = gain; // +/- 10
        const double midFrequency = freq;
        const double midQuality = q;
        recursive_linear_filter::BiquadParams midParams(sampleRate, midFrequency, midQuality, midGainDB);

        mBellFilter.SetParams(midParams);

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
            mVerbDoubleBuffer = new double* [isMono];
            for (int channel = 0; channel < isMono; ++channel)
            {
                mDoubleBuffer[channel] = new double[1024];
                mTempDoubleBuffer[channel] = new double[1024];
                mVerbDoubleBuffer[channel] = new double[1024];
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
        const int targetBitDepth = pBitDepth->get(); // Desired bit depth
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
            float numOfSamplesForOneValue = getSampleRate() / pSampleRate->get();
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
            const float inputGainDb = pGainSat->get(); // Adjust this value as needed
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
        float mix = pSatMix->get();
        for (int channel = 0; channel < isMono; channel++)
        {
            for (int s = 0; s < numSamples; s++)
            {
                mDoubleBuffer[channel][s] = (mDoubleBuffer[channel][s] * (mix) + mTempDoubleBuffer[channel][s] * (100-mix)) * 0.01; // Carefull who get the mix
            }
        }


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
        }

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
    }

    //==============================================================================
    AudioProcessorEditor* createEditor() override          { return new GenericAudioProcessorEditor (*this); }
    bool hasEditor() const override                        { return true;   }

    //==============================================================================
    const String getName() const override                  { return "Gain PlugIn"; }
    bool acceptsMidi() const override                      { return false; }
    bool producesMidi() const override                     { return false; }
    double getTailLengthSeconds() const override           { return 0; }

    //==============================================================================
    int getNumPrograms() override                          { return 1; }
    int getCurrentProgram() override                       { return 0; }
    void setCurrentProgram (int) override                  {}
    const String getProgramName (int) override             { return "None"; }
    void changeProgramName (int, const String&) override   {}
    void parameterValueChanged(int parameterIndex, float newValue) override
    {
        if (parameterIndex == pRadiationLength->getParameterIndex())
        {
			mGeigerLengthSample = static_cast<int>(newValue * getSampleRate());
		}
        if (parameterIndex == pRadiationsFrequence->getParameterIndex())
        {
            float realValue = pRadiationsFrequence->range.start + newValue * (pRadiationsFrequence->range.end - pRadiationsFrequence->range.start);
            dist = std::exponential_distribution<float>(realValue);
        }

    };
    void parameterGestureChanged(int, bool) override {}
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override
    {
        MemoryOutputStream (destData, true).writeFloat (*gain);
    }

    void setStateInformation (const void* data, int sizeInBytes) override
    {
        gain->setValueNotifyingHost (MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat());
    }


    //==============================================================================
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
    {
        const auto& mainInLayout  = layouts.getChannelSet (true,  0);
        const auto& mainOutLayout = layouts.getChannelSet (false, 0);

        return (mainInLayout == mainOutLayout && (! mainInLayout.isDisabled()));
    }

private:
    //==============================================================================
    AudioParameterFloat* gain;
    AudioParameterInt* pBitDepth;
    AudioParameterInt* pSampleRate;

    AudioParameterChoice* pSampleRateAlgoChoice;

    AudioParameterFloat* pMix;
    AudioParameterFloat* pGainSat;
    AudioParameterFloat* pSatMix;

    AudioParameterFloat* pSatBellFreq;
    AudioParameterFloat* pSatBellGain;
    AudioParameterFloat* pSatBellQ;

    recursive_linear_filter::Peaking mBellFilter;
    std::uniform_real_distribution<float> mDistribution_noise;

    // Use a threshold on the signal and hash it ?
    AudioParameterFloat* pRadiationsFrequence;
    AudioParameterFloat* pRadiationLength;
    AudioParameterFloat* pRadiationAmplitude;

    int mGeigerLengthSample = 44100;


    float** mFloatBuffer = nullptr;
    float** mTempFloatBuffer = nullptr;

    double** mDoubleBuffer = nullptr;
    double** mTempDoubleBuffer = nullptr;
    double** mVerbDoubleBuffer = nullptr;

    std::mt19937 rng;
    std::exponential_distribution<float> dist;
    float sampleRate;
    int nextImpulseSample;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainProcessor)
};
