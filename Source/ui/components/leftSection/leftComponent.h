
#pragma once
#include <JuceHeader.h>
#include "../../../dsp/Processor.h"

class LeftSection : public juce::Component
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

    LeftSection(juce::AudioProcessor& processor)
        : mProcessor(processor)
    {
        defineKnobLayout();
        configureNodes(processor);
        setSliderAttachement(processor);
    }

    ~LeftSection() override
	{
	}

    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colours::black); // Set the color for the bounds
        g.drawRect(getLocalBounds(), 1);   // Draw the rectangle with a thickness of 1 pixel
    }

    void setSliderAttachement(juce::AudioProcessor& inProcessor)
    {
        auto processor = dynamic_cast<SkeletonAudioProcessor*>(&inProcessor);
        mHarmKnobAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->getCustomParameterTree(), "radiationAmplitude", mHarmKnob);
		mTimeKnobAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->getCustomParameterTree(), "radiationLength", mTimeKnob);
		mDetectionKnobAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->getCustomParameterTree(), "radiationFrequency", mDetectionKnob);
    
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
        mTimeKnobLayout.inLayout.x = -30;
        mTimeKnobLayout.inLayout.y = -4;
		mTimeKnobLayout.inLayout.frameWidth = 164;
        mTimeKnobLayout.inLayout.frameHeight = 178;
        mTimeKnobLayout.inLayout.ratio = 1.f;


        mHarmKnobLayout.inLayout.x = 220;
		mHarmKnobLayout.inLayout.y = 4;
        mHarmKnobLayout.inLayout.frameWidth = 173;
		mHarmKnobLayout.inLayout.frameHeight = 181;
        mHarmKnobLayout.inLayout.ratio = 1.f;

		mDetectionKnobLayout.inLayout.x = 15;
        mDetectionKnobLayout.inLayout.y = 138;
		mDetectionKnobLayout.inLayout.frameWidth = 298;
        mDetectionKnobLayout.inLayout.frameHeight = 329;
        mDetectionKnobLayout.inLayout.ratio = 1.f;



        computeKnobLayout(mTimeKnobLayout);
        computeKnobLayout(mHarmKnobLayout);
        computeKnobLayout(mDetectionKnobLayout);
    }

    void configureNodes(juce::AudioProcessor& inProcessor)
	{
		mHarmKnob.setSliderStyle(juce::Slider::Rotary);
		mHarmKnob.setBounds(mHarmKnobLayout.outLayout.x, mHarmKnobLayout.outLayout.y, mHarmKnobLayout.outLayout.sliderWidth, mHarmKnobLayout.outLayout.sliderHeight);
		addAndMakeVisible(mHarmKnob);

		mTimeKnob.setSliderStyle(juce::Slider::Rotary);
		mTimeKnob.setBounds(mTimeKnobLayout.outLayout.x, mTimeKnobLayout.outLayout.y, mTimeKnobLayout.outLayout.sliderWidth, mTimeKnobLayout.outLayout.sliderHeight);
		addAndMakeVisible(mTimeKnob);

		mDetectionKnob.setSliderStyle(juce::Slider::Rotary);
		mDetectionKnob.setBounds(mDetectionKnobLayout.outLayout.x, mDetectionKnobLayout.outLayout.y, mDetectionKnobLayout.outLayout.sliderWidth, mDetectionKnobLayout.outLayout.sliderHeight);
		addAndMakeVisible(mDetectionKnob);

        mDetectionKnob.setLookAndFeel(new KnobLookAndFeel());
        
        auto look = dynamic_cast<KnobLookAndFeel*>(&mDetectionKnob.getLookAndFeel());
        look->setImage(BinaryData::gRad_png, BinaryData::gRad_pngSize);
        mDetectionKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

        mHarmKnob.setLookAndFeel(&mKnobLookAndFeel);
        auto look2 = dynamic_cast<KnobLookAndFeel*>(&mHarmKnob.getLookAndFeel());
        look2->setImage(BinaryData::g1_png, BinaryData::g1_pngSize);
		mHarmKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

        mTimeKnob.setLookAndFeel(&mKnobLookAndFeel);
        auto look3 = dynamic_cast<KnobLookAndFeel*>(&mTimeKnob.getLookAndFeel());
		look3->setImage(BinaryData::g2_png, BinaryData::g2_pngSize);
		mTimeKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

        
	}

private:
    juce::AudioProcessor& mProcessor;

    juce::Slider mHarmKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mHarmKnobAttachement;
    KnobLayout mHarmKnobLayout;

    juce::Slider mTimeKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mTimeKnobAttachement;
    KnobLayout mTimeKnobLayout;

    juce::Slider mDetectionKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDetectionKnobAttachement;
    KnobLayout mDetectionKnobLayout;

    KnobLookAndFeel mKnobLookAndFeel;

};