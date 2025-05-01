#include "gui.h"

#include "../dsp/Processor.h"
#include <JuceHeader.h>
int RootViewComponent::ROOT_WIDTH = 980;
int RootViewComponent::ROOT_HEIGHT = 550;

RootViewComponent::RootViewComponent(juce::AudioProcessor& processor)
    : AudioProcessorEditor(processor)
    , mProcessor(processor)
    , mTopBar(processor)
    , mLeftSection(processor)
{
    auto& gainProcessor = processor; 

    auto imageData = BinaryData::plate_png;
    auto imageDataSize = BinaryData::plate_pngSize;

    mImage = juce::ImageFileFormat::loadFrom(imageData, imageDataSize);

    if (mImage.isNull())
        DBG("Failed to load image from resources");

    setSize(mImage.getBounds().getWidth(), mImage.getBounds().getHeight());
    defineKnobLayout();
    configureNodes(gainProcessor);
}

RootViewComponent::~RootViewComponent()
{
	
	mMidKnob.setLookAndFeel(nullptr);

}

void RootViewComponent::setSliderAttachement(AudioProcessor& inProcessoe)
{
    SkeletonAudioProcessor* ampAudioProcessor = dynamic_cast<SkeletonAudioProcessor*>(&inProcessoe);
  //  mInputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
    //    ampAudioProcessor->getCustomParameterTree(), "input", mInputKnob);
 
}

void RootViewComponent::updatePath()
{
   // SkeletonAudioProcessor* ampAudioProcessor = dynamic_cast<SkeletonAudioProcessor*>(&processor);

}

void RootViewComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    if (!mImage.isNull())
    {
        auto bounds = getLocalBounds().toFloat();
        auto imageBounds = mImage.getBounds().toFloat();
        auto scale = juce::jmin(bounds.getWidth() / imageBounds.getWidth(),
            bounds.getHeight() / imageBounds.getHeight());
        auto scaledImageBounds = imageBounds.withSize(imageBounds.getWidth() * scale,
            imageBounds.getHeight() * scale);
        g.drawImage(mImage, scaledImageBounds);
    }
    else
    {
        g.setColour(juce::Colours::white);
        g.drawText("Image not found", getLocalBounds(), juce::Justification::centred);
    }
}

void RootViewComponent::resized()
{
    auto bounds = getLocalBounds();
}

void RootViewComponent::MeterComponent::paint(juce::Graphics& g)
{
    SkeletonAudioProcessor& meter = static_cast<SkeletonAudioProcessor&>(audioProcessor);
    float rmsLeft = 0.0f;
    float rmsRight = 0.0f;

    if (isInput)
    {
        rmsLeft = meter.getRmsLevelLeft();
        rmsRight = meter.getRmsLevelRight();
        g.setColour(juce::Colours::white);
        rmsLeft = rmsLeft * 10;
    }
    else
    {
        rmsLeft = meter.getRmsOutputLevelLeft();
        rmsRight = meter.getRmsOutputLevelRight();
        g.setColour(juce::Colours::green);
        rmsLeft = rmsLeft * 10;
    }

    juce::Rectangle<float> meterBounds(0, 0, getWidth(), getHeight());
    g.setColour(juce::Colours::darkgrey.withAlpha(0.8f)); 
    g.fillRoundedRectangle(meterBounds, 5.0f); 

    juce::Rectangle<float> leftMeterBounds(10, getHeight() - (rmsLeft * getHeight()), getWidth() / 2 - 15, rmsLeft * getHeight());
    juce::ColourGradient leftGradient(juce::Colours::green, leftMeterBounds.getBottomLeft(),
        juce::Colours::limegreen, leftMeterBounds.getTopLeft(), false);
    g.setGradientFill(leftGradient);
    g.fillRoundedRectangle(leftMeterBounds, 3.0f); 

    juce::Rectangle<float> rightMeterBounds(getWidth() / 2 + 5, getHeight() - (rmsLeft * getHeight()), getWidth() / 2 - 15, rmsLeft * getHeight());
    juce::ColourGradient rightGradient(juce::Colours::green, rightMeterBounds.getBottomLeft(),
        juce::Colours::limegreen, rightMeterBounds.getTopLeft(), false);
    g.setGradientFill(rightGradient);
    g.fillRoundedRectangle(rightMeterBounds, 3.0f); 

    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.drawRoundedRectangle(meterBounds, 5.0f, 1.0f);
}

