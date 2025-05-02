#pragma once
#include <JuceHeader.h>
#include "Bones/ToneStack.h"
#include "dsp/NoiseGate.h"
#include "dsp/ImpulseResponse.h"
#include "ParameterSetup.h"
#include "Mappers.h"
#include "dsp/RecursiveLinearFilter.h"
#include <random>
#include "Processor.hpp"
//==============================================================================
class SkeletonAudioProcessor final : public AudioProcessor
{
public:

    //==============================================================================
    SkeletonAudioProcessor(juce::AudioProcessorValueTreeState& inParameters, ParameterSetup& inParameterSetup);


    ~SkeletonAudioProcessor() override;


    void prepareToPlay(double, int) override
    {
        mSampleRate = getSampleRate();
		mBlockSize = getBlockSize();
        Mappers::getMapperInstance().setSampleRate(mSampleRate);
    }
    void releaseResources() override {}
    void processBlock(AudioBuffer<float>& buffer, MidiBuffer&) override;
    void updateMeter(bool isOutput, AudioBuffer<float>& buffer, int numSamples, int numChannels);

    //==============================================================================
    AudioProcessorEditor* createEditor() override {
        return nullptr;
    }
    bool hasEditor() const override { return false; }

    //==============================================================================
    const String getName() const override { return "Radiator"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0; }

    //==============================================================================
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const String getProgramName(int) override { return "None"; }
    void changeProgramName(int, const String&) override {}


    double getRmsLevelLeft() const { return mRmsLevelLeft.load(); }
    double getRmsLevelRight() const { return mRmsLevelRight.load(); }
    double getRmsOutputLevelLeft() const { return mRmsOutputLevelLeft.load(); }
    double getRmsOutputLevelRight() const { return mRmsOutputLevelRight.load(); }

    //==============================================================================
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override
    {
        return true;
        const auto& mainInLayout = layouts.getChannelSet(true, 0);
        const auto& mainOutLayout = layouts.getChannelSet(false, 0);

        return (mainInLayout == mainOutLayout && (!mainInLayout.isDisabled()));
    }

    juce::AudioProcessorValueTreeState& SkeletonAudioProcessor::getCustomParameterTree()
    {
        return mParameters;
    }



    void initState()
    {
        
    }

    void getStateInformation(MemoryBlock& destData) override
    {
	}

    void setStateInformation(const void* data, int sizeInBytes) override
    {
		juce::MemoryInputStream stream(data, static_cast<size_t>(sizeInBytes), false);
		auto newState = juce::ValueTree::readFromStream(stream);
	}

	//==============================================================================
    void setRateAndBufferSizeDetails(double sampleRate, int bufferSize)
    {
		mSampleRate = sampleRate;
		mBlockSize = bufferSize;
    }
private:
    //==============================================================================
    juce::AudioProcessorValueTreeState& mParameters;
    ParameterSetup& mParameterSetup;
    dsp::tone_stack::AbstractToneStack* mToneStack;


    recursive_linear_filter::Peaking mBellFilter;
    std::uniform_real_distribution<float> mDistribution_noise;

    float** mFloatBuffer = nullptr;
    float** mTempFloatBuffer = nullptr;

    double** mDoubleBuffer = nullptr;
    double** mTempDoubleBuffer = nullptr;

    ParamListener mParamListener;

private:
    std::atomic<float> mRmsLevelLeft{ 0.0f };
    std::atomic<float> mRmsLevelRight{ 0.0f };
    std::atomic<float> mRmsOutputLevelLeft{ 0.0f };
    std::atomic<float> mRmsOutputLevelRight{ 0.0f };

    double mBlockSize;
    double mSampleRate;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SkeletonAudioProcessor)
};
