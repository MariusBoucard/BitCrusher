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
#include <juce_dsp/juce_dsp.h>
//==============================================================================
class GainProcessor final : public AudioProcessor
{
public:

    //==============================================================================
    GainProcessor()
        : AudioProcessor (BusesProperties().withInput  ("Input",  AudioChannelSet::stereo())
                                           .withOutput ("Output", AudioChannelSet::stereo()))
    {
        addParameter (gain = new AudioParameterFloat ({ "gain", 1 }, "Gain", 0.0f, 1.0f, 0.5f));
        addParameter(pBitDepth = new AudioParameterInt("bitDepth", "bitDepth", 1,16,16));
        addParameter(pSampleRate = new AudioParameterInt("sampleRate", "SampleRate", 1, 96000, 44100));

        addParameter(pSampleRateAlgoChoice = new AudioParameterChoice("downsampleAlgo", "Downsample Algorithm", { "min", "max", "rms" }, 1));

        addParameter(pGainSat = new AudioParameterFloat("gainSat", "Saturation gain", -12, 12, 0));
        addParameter(pSatMix = new AudioParameterFloat("mixSat", "Saturation mix", 0.f, 100.f, 100.f));

        addParameter(pSatBellFreq = new AudioParameterFloat("satBellFreq", "Sat Boost Freq", 10.f, 18000.f, 1000.f));
        addParameter(pSatBellGain = new AudioParameterFloat("satBellGain", "Sat Boost Gain", -12.f, 12.f, 0.f));
        addParameter(pSatBellQ = new AudioParameterFloat("satBellQ", "Sat Bell Queue", 0.f, 7.f, 3.f));

        addParameter(pMix = new AudioParameterFloat("mix", "Mix", 0.f, 100.f, 100.f));

        // Use a threshold on the signal and hash it ?
        AudioParameterFloat* pRadiationsFrequence;
        AudioParameterFloat* pRadiationLength;


    }


    //==============================================================================
    void prepareToPlay (double, int) override {
       auto sampleRate = getSampleRate();
       auto coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, pSatBellFreq->get(), pSatBellQ->get(), juce::Decibels::decibelsToGain(pSatBellGain->get()));
       mBellFilter.coefficients = coefficients;
    }

    void releaseResources() override {}

    void processBlock (AudioBuffer<float>& buffer, MidiBuffer&) override
    {
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
        for (int channel = 0; channel < isMono; ++channel)
        {
            for (int sample = 0; sample < numSamples; ++sample)
            {
                const int targetBitDepth = pBitDepth->get(); // Change this to your desired bit depth
                const double maxAmplitude = std::pow(2.0, targetBitDepth - 1) - 1;
                mTempDoubleBuffer[channel][sample] = std::round(mDoubleBuffer[channel][sample] * maxAmplitude) / maxAmplitude;
            }
        }

        // SampleRate

        for (int channel = 0; channel < isMono; ++channel)
        {
            // Need a buffer
            float numOfSamplesForOneValue = getSampleRate() / pSampleRate->get();
            numOfSamplesForOneValue > numSamples ? numOfSamplesForOneValue = numSamples : numOfSamplesForOneValue = numOfSamplesForOneValue;
            int minS = int(numOfSamplesForOneValue);
            // To be move for continuity for all arrays
            int channelSign = -1;
            int processedSamples = 0;
            if (numSamples - processedSamples > minS)
            {
                double absMaxSaw = -100;
                for (int s = 0; s < minS; s++)
                {
                    abs(mTempDoubleBuffer[channel][s])> absMaxSaw ? absMaxSaw = abs(mTempDoubleBuffer[channel][s]) : absMaxSaw = absMaxSaw ;
                }
                for (int s = 0; s < minS; s++)
                {
                    mDoubleBuffer[channel][s] = absMaxSaw;
                }
            }
            else
            {
                // Partial value fuck it
                double absMaxSaw = -100;
                for (int s = 0; s < numSamples - processedSamples; s++)
                {
                    abs(mTempDoubleBuffer[channel][s]) > absMaxSaw ? absMaxSaw = abs(mTempDoubleBuffer[channel][s]) : absMaxSaw = absMaxSaw;
                }
                for (int s = 0; s < minS; s++)
                {
                    mDoubleBuffer[channel][s] = absMaxSaw;
                }
            }
   
        }

        // BIFBOF
        auto coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), pSatBellFreq->get(), pSatBellQ->get(), juce::Decibels::decibelsToGain(pSatBellGain->get()));
        mBellFilter.coefficients = coefficients;
        // Put float so carefull with buffer 
        
        for (int channel = 0; channel < numChannels; channel++)
        {
            for (int s = 0; s < numSamples; s++)
            {
                mFloatBuffer[0][s] = mDoubleBuffer[channel][s];
            }

            juce::dsp::AudioBlock<float> audioBlock(mFloatBuffer, 1, static_cast<size_t>(numSamples));

            juce::dsp::ProcessContextReplacing<float> context(audioBlock);

            mBellFilter.process(context);

            for (int s = 0; s < numSamples; s++)
            {
                mDoubleBuffer[channel][s] = mFloatBuffer[0][s];
            }

        }
        // Saturation 
        //for (int channel = 0; channel < isMono; ++channel)
        //{
        //    const float inputGainDb = pGainSat->get(); // Adjust this value as needed
        //    const double inputGainLinear = std::pow(10.0, inputGainDb / 20.0); // Convert dB to linear scale


        //    std::transform(mDoubleBuffer[channel], mDoubleBuffer[channel] + numSamples, mTempDoubleBuffer[channel],
        //        [inputGainLinear](double sample) {
        //            sample *= inputGainLinear;

        //            // Smooth buffy in lows
        //            // sample = std::tanh(sample);
        //            // sample = std::atan(sample); // Arctangent saturation
        //            // sample = std::sin(sample); // Sine wave saturation
        //            // sample = sample - (1.5 * std::pow(sample, 3)); // Polynomial saturation
        //            // sample = sample / (1.0 + std::abs(sample * 2.0)); // Wave shaping
        //    // 
        //            // HARD CLIP -> way more agressive
        //            const double threshold = 0.8; // Clipping threshold
        //            if (sample > threshold) sample = threshold;
        //            else if (sample < -threshold) sample = -threshold;
        //            //

        //            sample /= inputGainLinear;

        //            return sample;
        //        });
        //}

        // Sat mix
        //float mix = pSatMix->get();
        //for (int channel = 0; channel < isMono; channel++)
        //{
        //    for (int s = 0; s < numSamples; s++)
        //    {
        //        mDoubleBuffer[channel][s] = (mDoubleBuffer[channel][s] * (100-mix) + mTempDoubleBuffer[channel][s] * mix) * 0.01; // Get back between 0 and 1
        //    }
        //}

        for (int channel = 0; channel < isMono; ++channel)
        {
            std::transform(mDoubleBuffer[channel], mDoubleBuffer[channel] + numSamples, mTempFloatBuffer[channel], [](double sample) {
                        return static_cast<float>(sample);
                        });
        }
  /*      for (int channel = 0; channel < isMono; ++channel)
        {
            auto* floatData = buffer.getWritePointer(channel);
            std::copy(mTempFloatBuffer[channel], mTempFloatBuffer[channel] + numSamples, floatData);
        }*/


    }

    //void processBlock (AudioBuffer<double>& buffer, MidiBuffer&) override
    //{
    //    buffer.applyGain ((float) *gain);
    //}

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

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override
    {
        MemoryOutputStream (destData, true).writeFloat (*gain);
    }

    void setStateInformation (const void* data, int sizeInBytes) override
    {
        gain->setValueNotifyingHost (MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat());
    }
    void parameterChanged(const juce::String& parameterID, float newValue)
    {
        if (parameterID == "satBellFreq" || parameterID == "satBellGain" || parameterID == "satBellQ")
        {
            float frequency = pSatBellFreq->get();
            float gain = juce::Decibels::decibelsToGain(pSatBellGain->get());
            float q = pSatBellQ->get();

            auto coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), frequency, q, gain);
            *mBellFilter.coefficients = *coefficients;
        }
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
    juce::dsp::IIR::Filter<float> mBellFilter;

    // Use a threshold on the signal and hash it ?
    AudioParameterFloat* pRadiationsFrequence;
    AudioParameterFloat* pRadiationLength;


    float** mFloatBuffer = nullptr;
    float** mTempFloatBuffer = nullptr;

    double** mDoubleBuffer = nullptr;
    double** mTempDoubleBuffer = nullptr;
    double** mVerbDoubleBuffer = nullptr;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainProcessor)
};
