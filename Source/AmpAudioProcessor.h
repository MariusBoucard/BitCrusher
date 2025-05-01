
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
#include "ui/gui.h"
#include "dsp/Bones/ToneStack.h"
#include "dsp/NoiseGate.h"
#include "dsp/ImpulseResponse.h"
#include "dsp/Processor.h"
#include "dsp/ParameterSetup.h"

class AmpAudioProcessor final : public AudioProcessor
{
public:

    AmpAudioProcessor();
    ~AmpAudioProcessor() override;

    juce::AudioProcessorValueTreeState::ParameterLayout AmpAudioProcessor::createParameterLayout()
    {
        std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

        params.push_back(std::make_unique<juce::AudioParameterFloat>("gain", "Gain", 0.0f, 1.0f, 0.5f));
        params.push_back(std::make_unique<juce::AudioParameterInt>("bitDepth", "Bit Depth", 1, 16, 16));
        params.push_back(std::make_unique<juce::AudioParameterInt>("sampleRate", "Sample Rate", 1, 96000, 44100));
        params.push_back(std::make_unique<juce::AudioParameterChoice>("satAlgo", "Saturation Algorithm", 
            juce::StringArray{ "Soft Clip", "Hard Clip", "Wave shaping", "Polynomial saturation", "Tanh", "ArctanH" }, 1));
        params.push_back(std::make_unique<juce::AudioParameterFloat>("gainSat", "Saturation Gain", -12.0f, 12.0f, 0.0f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>("mixSat", "Saturation Mix", 0.0f, 100.0f, 100.0f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>("satBellFreq", "Sat Boost Freq", 10.0f, 18000.0f, 1000.0f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>("satBellGain", "Sat Boost Gain", -12.0f, 12.0f, 0.0f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>("satBellQ", "Sat Bell Q", 0.0f, 7.0f, 3.0f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>("mix", "Mix", 0.0f, 100.0f, 100.0f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>("radiationAmplitude", "Radiation Amplitude", 0.0f, 1.0f, 0.5f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>("radiationFrequency", "Radiation Frequency", 0.1f, 50.0f, 0.5f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>("radiationLength", "Radiation Length", 0.0f, 1.0f, 0.5f));
    

        return { params.begin(), params.end() };
    }

    void prepareToPlay (double, int) override {}
    void releaseResources() override {}

    void processBlock(AudioBuffer<float>& buffer, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override {
        auto editor = new RootViewComponent(mSkeletonAmpProcessor);
        editor->updatePath();
        return editor;
    }
    bool hasEditor() const override                        { return true;   }

    const String getName() const override                  { return "Ballzzy's NAM"; }
    bool acceptsMidi() const override                      { return false; }
    bool producesMidi() const override                     { return false; }
    double getTailLengthSeconds() const override           { return 0; }

    int getNumPrograms() override                          { return 1; }
    int getCurrentProgram() override                       { return 0; }
    void setCurrentProgram (int) override                  {}
    const String getProgramName (int) override             { return "None"; }
    void changeProgramName (int, const String&) override   {}


    void getStateInformation (MemoryBlock& destData) override
    {
        juce::MemoryOutputStream stream(destData, true);
        mParameters.state.writeToStream(stream);  
    }

    juce::File AmpAudioProcessor::createJucePathFromFile(const juce::String& filePath);

    void setStateInformation (const void* data, int sizeInBytes) override
    {
        juce::MemoryInputStream stream(data, static_cast<size_t>(sizeInBytes), false);
        auto newState = juce::ValueTree::readFromStream(stream);

        if (newState.isValid())
        {
            mParameters.state = newState;
            mSkeletonAmpProcessor.initState();

            if(getActiveEditor() != nullptr)
			{
                RootViewComponent* rootView = dynamic_cast<RootViewComponent*>(getActiveEditor());
				rootView->updatePath();
			}  
        }
    }

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
    {
        return true;
        const auto& mainInLayout  = layouts.getChannelSet (true,  0);
        const auto& mainOutLayout = layouts.getChannelSet (false, 0);

        return (mainInLayout == mainOutLayout && (! mainInLayout.isDisabled()));
    }

    juce::AudioProcessorValueTreeState& AmpAudioProcessor::getCustomParameterTree()
    {
        return mParameters;
    }

private:
    juce::AudioProcessorValueTreeState mParameters; 
    SkeletonAudioProcessor mSkeletonAmpProcessor; 
    ParameterSetup mParameterSetup;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmpAudioProcessor)
};
