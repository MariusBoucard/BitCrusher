
#pragma once
#include <JuceHeader.h>
#include "../../../dsp/Processor.h"

class MidSection : public juce::Component
{
public:
    struct KnobLayout {
        struct input {
            int x;
            int y;
            int frameWidth;
            int frameHeight;
            float ratio;
            int textboxHeight;
            int textboxPadding;
        };
        struct output {
            int x;
            int y;
            int sliderWidth;
            int sliderHeight;
        };
        input inLayout;
        output outLayout;
    };

    MidSection(juce::AudioProcessor& processor)
        : mProcessor(processor)
    {
        defineKnobLayout();
        configureNodes(processor);
        setSliderAttachement(processor);
    }

    ~MidSection() override
	{
	}

    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colours::black); // Set the color for the bounds
    //    g.drawRect(getLocalBounds(), 1);   // Draw the rectangle with a thickness of 1 pixel
    }

    void setSliderAttachement(juce::AudioProcessor& inProcessor)
    {
        auto processor = dynamic_cast<SkeletonAudioProcessor*>(&inProcessor);
        mBitKnobAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->getCustomParameterTree(), "bitDepth", mBitKnob);
		mSRKnobAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->getCustomParameterTree(), "sampleRate", mSRKnob);
		mRadKnobAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->getCustomParameterTree(), "radiationFrequency", mRadKnob);
    
    }

    void computeKnobLayout(KnobLayout& inKnobLayout)
    {
        inKnobLayout.outLayout.x = inKnobLayout.inLayout.x;
        inKnobLayout.outLayout.y = inKnobLayout.inLayout.y;
        inKnobLayout.outLayout.sliderWidth = inKnobLayout.inLayout.frameWidth * inKnobLayout.inLayout.ratio;
        inKnobLayout.outLayout.sliderHeight = inKnobLayout.inLayout.frameHeight * inKnobLayout.inLayout.ratio;
    }


    void resized() override
    {
    }

    void defineKnobLayout()
    {
        mBitKnobLayout.inLayout.x = 46;
        mBitKnobLayout.inLayout.y = 99;
        mBitKnobLayout.inLayout.frameWidth = 233;
        mBitKnobLayout.inLayout.frameHeight = 269;
        mBitKnobLayout.inLayout.ratio = 1.f;

        mSRKnobLayout.inLayout.x = 664;
        mSRKnobLayout.inLayout.y = 78;
        mSRKnobLayout.inLayout.frameWidth = 257;
        mSRKnobLayout.inLayout.frameHeight = 323;
        mSRKnobLayout.inLayout.ratio = 1.f;

        mRadKnobLayout.inLayout.x = 91;
        mRadKnobLayout.inLayout.y = 200;
        mRadKnobLayout.inLayout.frameWidth = 778;
        mRadKnobLayout.inLayout.frameHeight = 689;
        mRadKnobLayout.inLayout.ratio = 1.f;


        computeKnobLayout(mBitKnobLayout);
        computeKnobLayout(mSRKnobLayout);
        computeKnobLayout(mRadKnobLayout);
    }

    void configureNodes(juce::AudioProcessor& inProcessor)
	{
		mBitKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
		mBitKnob.setBounds(mBitKnobLayout.outLayout.x, mBitKnobLayout.outLayout.y, mBitKnobLayout.outLayout.sliderWidth, mBitKnobLayout.outLayout.sliderHeight);
		addAndMakeVisible(mBitKnob);

        mBitKnob.setLookAndFeel(new KnobLookAndFeel());
        auto look = dynamic_cast<KnobLookAndFeel*>(&mBitKnob.getLookAndFeel());
        look->setImage(BinaryData::bit_png, BinaryData::bit_pngSize);
        mBitKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

        mSRKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        mSRKnob.setBounds(mSRKnobLayout.outLayout.x, mSRKnobLayout.outLayout.y, mSRKnobLayout.outLayout.sliderWidth, mSRKnobLayout.outLayout.sliderHeight);
        addAndMakeVisible(mSRKnob);

        mSRKnob.setLookAndFeel(new KnobLookAndFeel());
        auto look2 = dynamic_cast<KnobLookAndFeel*>(&mSRKnob.getLookAndFeel());
        look2->setImage(BinaryData::sr_png, BinaryData::sr_pngSize);
        mSRKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        
        mRadKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        mRadKnob.setBounds(mRadKnobLayout.outLayout.x, mRadKnobLayout.outLayout.y, mRadKnobLayout.outLayout.sliderWidth, mRadKnobLayout.outLayout.sliderHeight);
        addAndMakeVisible(mRadKnob);

        mRadKnob.setLookAndFeel(new KnobLookAndFeel());
        auto look3 = dynamic_cast<KnobLookAndFeel*>(&mRadKnob.getLookAndFeel());
        look3->setImage(BinaryData::RAD_png, BinaryData::RAD_pngSize);
        mRadKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        
	}

private:
    juce::AudioProcessor& mProcessor;

    juce::Slider mBitKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mBitKnobAttachement;
    KnobLayout mBitKnobLayout;

    juce::Slider mSRKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mSRKnobAttachement;
    KnobLayout mSRKnobLayout;

    juce::Slider mRadKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mRadKnobAttachement;
    KnobLayout mRadKnobLayout;

    KnobLookAndFeel mKnobLookAndFeel;

};