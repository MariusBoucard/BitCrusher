
#pragma once
#include <JuceHeader.h>
#include "../../../dsp/Processor.h"

class RightSection : public juce::Component
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

    RightSection(juce::AudioProcessor& processor)
        : mProcessor(processor)
    {
        defineKnobLayout();
        configureNodes(processor);
        setSliderAttachement(processor);
    }

    ~RightSection() override
	{
	}

    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colours::black); // Set the color for the bounds
      //  g.drawRect(getLocalBounds(), 1);   // Draw the rectangle with a thickness of 1 pixel
    }

    void setSliderAttachement(juce::AudioProcessor& inProcessor)
    {
        auto processor = dynamic_cast<SkeletonAudioProcessor*>(&inProcessor);
        mSatGainKnobAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->getCustomParameterTree(), "gainSat", mSatGainKnob);
        mSatMixKnobAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->getCustomParameterTree(), "mixSat", mSatMixKnob);
        mBFKnobAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->getCustomParameterTree(), "satBellFreq", mBFKnob);
        mBGKnobAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->getCustomParameterTree(), "satBellGain", mBGKnob);
        mBQGainKnobAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->getCustomParameterTree(), "satBellQ", mBQGainKnob);
        mMixKnobAttachement = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->getCustomParameterTree(), "mix", mMixKnob);

    
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
        mSatGainKnobLayout.inLayout.x = -7;
        mSatGainKnobLayout.inLayout.y = 167;
        mSatGainKnobLayout.inLayout.frameWidth =274;
        mSatGainKnobLayout.inLayout.frameHeight = 240;
        mSatGainKnobLayout.inLayout.ratio = 1.f;

        mSatMixKnobLayout.inLayout.x = 240;
        mSatMixKnobLayout.inLayout.y = 193;
        mSatMixKnobLayout.inLayout.frameWidth = 170;
        mSatMixKnobLayout.inLayout.frameHeight = 220;
        mSatMixKnobLayout.inLayout.ratio = 1.f;

        mBFKnobLayout.inLayout.x = 27;
        mBFKnobLayout.inLayout.y = 434;
        mBFKnobLayout.inLayout.frameWidth = 138;
        mBFKnobLayout.inLayout.frameHeight = 185;
        mBFKnobLayout.inLayout.ratio = 1.f;

        mBGKnobLayout.inLayout.x = 163;
        mBGKnobLayout.inLayout.y = 366;
        mBGKnobLayout.inLayout.frameWidth = 130;
        mBGKnobLayout.inLayout.frameHeight = 193;
        mBGKnobLayout.inLayout.ratio = 1.f;

        mBQGainKnobLayout.inLayout.x = 290;
        mBQGainKnobLayout.inLayout.y = 445;
        mBQGainKnobLayout.inLayout.frameWidth = 134;
        mBQGainKnobLayout.inLayout.frameHeight = 171;
        mBQGainKnobLayout.inLayout.ratio = 1.f;

        mMixKnobLayout.inLayout.x = 32;
        mMixKnobLayout.inLayout.y = 560;
        mMixKnobLayout.inLayout.frameWidth =300;
        mMixKnobLayout.inLayout.frameHeight = 253;
        mMixKnobLayout.inLayout.ratio = 1.f;

        computeKnobLayout(mSatGainKnobLayout);
        computeKnobLayout(mSatMixKnobLayout);
        computeKnobLayout(mBFKnobLayout);
        computeKnobLayout(mBGKnobLayout);
        computeKnobLayout(mBQGainKnobLayout);
        computeKnobLayout(mMixKnobLayout);

    }

    void configureNodes(juce::AudioProcessor& inProcessor)
	{
		mSatGainKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        mSatGainKnob.setBounds(mSatGainKnobLayout.outLayout.x, mSatGainKnobLayout.outLayout.y, mSatGainKnobLayout.outLayout.sliderWidth, mSatGainKnobLayout.outLayout.sliderHeight);
		addAndMakeVisible(mSatGainKnob);

        mSatGainKnob.setLookAndFeel(new KnobLookAndFeel());
        auto look = dynamic_cast<KnobLookAndFeel*>(&mSatGainKnob.getLookAndFeel());
        look->setImage(BinaryData::satG_png, BinaryData::satG_pngSize);
        mSatGainKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

        mSatMixKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        mSatMixKnob.setBounds(mSatMixKnobLayout.outLayout.x, mSatMixKnobLayout.outLayout.y, mSatMixKnobLayout.outLayout.sliderWidth, mSatMixKnobLayout.outLayout.sliderHeight);
        addAndMakeVisible(mSatMixKnob);
        mSatMixKnob.setLookAndFeel(new KnobLookAndFeel());
        auto look2 = dynamic_cast<KnobLookAndFeel*>(&mSatMixKnob.getLookAndFeel());
        look2->setImage(BinaryData::SatM_png, BinaryData::SatM_pngSize);
        mSatMixKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

        mBFKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        mBFKnob.setBounds(mBFKnobLayout.outLayout.x, mBFKnobLayout.outLayout.y, mBFKnobLayout.outLayout.sliderWidth, mBFKnobLayout.outLayout.sliderHeight);
        addAndMakeVisible(mBFKnob);
        mBFKnob.setLookAndFeel(new KnobLookAndFeel());
        auto look3 = dynamic_cast<KnobLookAndFeel*>(&mBFKnob.getLookAndFeel());
        look3->setImage(BinaryData::SatBF_png, BinaryData::SatBF_pngSize);
        mBFKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

        mBGKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        mBGKnob.setBounds(mBGKnobLayout.outLayout.x, mBGKnobLayout.outLayout.y, mBGKnobLayout.outLayout.sliderWidth, mBGKnobLayout.outLayout.sliderHeight);
        addAndMakeVisible(mBGKnob);
        mBGKnob.setLookAndFeel(new KnobLookAndFeel());
        auto look4 = dynamic_cast<KnobLookAndFeel*>(&mBGKnob.getLookAndFeel());
        look4->setImage(BinaryData::SatBG_png, BinaryData::SatBG_pngSize);
        mBGKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

        mBQGainKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        mBQGainKnob.setBounds(mBQGainKnobLayout.outLayout.x, mBQGainKnobLayout.outLayout.y, mBQGainKnobLayout.outLayout.sliderWidth, mBQGainKnobLayout.outLayout.sliderHeight);
        addAndMakeVisible(mBQGainKnob);
        mBQGainKnob.setLookAndFeel(new KnobLookAndFeel());
        auto look5 = dynamic_cast<KnobLookAndFeel*>(&mBQGainKnob.getLookAndFeel());
        look5->setImage(BinaryData::SatBQ_png, BinaryData::SatBQ_pngSize);
        mBQGainKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        
        mMixKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        mMixKnob.setBounds(mMixKnobLayout.outLayout.x, mMixKnobLayout.outLayout.y, mMixKnobLayout.outLayout.sliderWidth, mMixKnobLayout.outLayout.sliderHeight);
        addAndMakeVisible(mMixKnob);
        mMixKnob.setLookAndFeel(new KnobLookAndFeel());
        auto look6 = dynamic_cast<KnobLookAndFeel*>(&mMixKnob.getLookAndFeel());
        look6->setImage(BinaryData::Mix_png, BinaryData::Mix_pngSize);

	}

private:
    juce::AudioProcessor& mProcessor;



    juce::Slider mSatGainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mSatGainKnobAttachement;
    KnobLayout mSatGainKnobLayout;

    juce::Slider mSatMixKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mSatMixKnobAttachement;
    KnobLayout mSatMixKnobLayout;

    juce::Slider mBFKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mBFKnobAttachement;
    KnobLayout mBFKnobLayout;

    juce::Slider mBGKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mBGKnobAttachement;
    KnobLayout mBGKnobLayout;

    juce::Slider mBQGainKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mBQGainKnobAttachement;
    KnobLayout mBQGainKnobLayout;


    juce::Slider mMixKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mMixKnobAttachement;
    KnobLayout mMixKnobLayout;

    KnobLookAndFeel mKnobLookAndFeel;

};