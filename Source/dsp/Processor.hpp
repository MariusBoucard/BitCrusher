class ParamListener : public juce::AudioProcessorValueTreeState::Listener
{
public:
    ParamListener(juce::AudioProcessor& inParentProcessor,dsp::tone_stack::AbstractToneStack* toneStack, ParameterSetup& inParameterSetup)
        :
         mProcessor(inParentProcessor)
        , mToneStack(toneStack)
        , mParameterSetup(inParameterSetup)
    {}

    void parameterChanged(const juce::String& parameterID, float newValue) override
    {

            Mappers::setToneStackMid(mParameterSetup.mMidParams, newValue);
            mToneStack->SetParam("middle", newValue);

    }
private:
    juce::AudioProcessor& mProcessor;
    dsp::tone_stack::AbstractToneStack* mToneStack;
    dsp::noise_gate::Gain* mNoiseGateGain;
    dsp::noise_gate::Trigger* mNoiseGateTrigger;
    ParameterSetup& mParameterSetup;
};