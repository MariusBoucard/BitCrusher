// #include <JuceHeader.h>

// juce::AudioProcessorValueTreeState::ParameterLayout AmpAudioProcessor::createParameterLayout()
// {
//     std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

//     params.push_back (std::make_unique<juce::AudioParameterFloat> ("input", "Input", 0.0f, 1.0f, 0.5f));
//     params.push_back(std::make_unique<juce::AudioParameterFloat>("gate", "Gate", 0.0f, 1.0f, 0.5f));
//     params.push_back(std::make_unique<juce::AudioParameterFloat>("bass", "Bass", 0.0f, 1.0f, 0.5f));
//     params.push_back(std::make_unique<juce::AudioParameterFloat>("mid", "Mid", 0.0f, 1.0f, 0.5f));
//     params.push_back(std::make_unique<juce::AudioParameterFloat>("high", "High", 0.0f, 1.0f, 0.5f));
//     params.push_back(std::make_unique<juce::AudioParameterFloat>("output", "Output", 0.0f, 1.0f, 0.5f));
//     params.push_back(std::make_unique<juce::AudioParameterBool>("irEnabled", "IREnabled", false));
//     params.push_back(std::make_unique<juce::AudioParameterBool>("namEnabled", "NAMEnabled", true));
//     params.push_back(std::make_unique<juce::AudioParameterBool>("irVerbEnabled", "IRVerbEnabled", false));
//     params.push_back(std::make_unique<juce::AudioParameterFloat>("irVerbMix", "irVerbMix", 0.0f, 1.0f, 0.5f));

//     return { params.begin(), params.end() };
// }