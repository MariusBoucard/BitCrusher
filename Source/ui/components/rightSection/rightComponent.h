
#pragma once
#include <JuceHeader.h>

class BoxComponent : public juce::Component
{
public:
BoxComponent(juce::AudioProcessor& processor)
        : audioProcessor(processor)
    {


    }

    ~BoxComponent() override
	{
	}

    void paint(juce::Graphics& g) override;

    void resized() override
    {
    }

private:
    juce::AudioProcessor& audioProcessor;




};