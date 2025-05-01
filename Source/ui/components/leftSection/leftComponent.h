
#pragma once
#include <JuceHeader.h>

class LeftSection : public juce::Component
{
public:
    LeftSection(juce::AudioProcessor& processor)
        : audioProcessor(processor)
    {


    }

    ~LeftSection() override
	{
	}

    void paint(juce::Graphics& g) override;

    void resized() override
    {
    }

private:
    juce::AudioProcessor& audioProcessor;




};