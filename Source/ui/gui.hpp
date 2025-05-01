

void RootViewComponent::defineKnobLayout()
{
    mTopBarLayout.inLayout.x = 0;
    mTopBarLayout.inLayout.y = 0;
    mTopBarLayout.inLayout.frameWidth = getLocalBounds().getWidth();
    mTopBarLayout.inLayout.frameHeight = 30;
    mTopBarLayout.inLayout.ratio = 1.f;
    mTopBarLayout.inLayout.textboxHeight = 0;
    mTopBarLayout.inLayout.textboxPadding = 0;



    computeKnobLayout(mInputMeterLayout);

}


void RootViewComponent::configureNodes(juce::AudioProcessor& inProcessor)
{
    auto bounds = getLocalBounds();

    mInputKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mBassKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mMidKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mHighKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mOutputKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mGateKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mVerbMixKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);

    mIRButton.setLookAndFeel(&mToggleLookAndFeel);
    mNAMButton.setLookAndFeel(&mToggleLookAndFeel);
    mIRVerbButton.setLookAndFeel(&mToggleLookAndFeel);

    mInputKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, mInputKnobLayout.inLayout.textboxHeight);
    mGateKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, mGateKnobLayout.inLayout.textboxHeight);
    mBassKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, mBassKnobLayout.inLayout.textboxHeight);
    mMidKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, mMidKnobLayout.inLayout.textboxHeight);
    mHighKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, mHighKnobLayout.inLayout.textboxHeight);
    mOutputKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, mOutputKnobLayout.inLayout.textboxHeight);
    mVerbMixKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, mVerbMixKnobLayout.inLayout.textboxHeight);

    setSliderAttachement(inProcessor);
   

    mInputKnob.setBounds(mInputKnobLayout.outLayout.x,
        mInputKnobLayout.outLayout.y,
        mInputKnobLayout.outLayout.sliderWidth,
        mInputKnobLayout.outLayout.sliderHeight);
    mGateKnob.setBounds(mGateKnobLayout.outLayout.x,
        mGateKnobLayout.outLayout.y,
        mGateKnobLayout.outLayout.sliderWidth,
        mGateKnobLayout.outLayout.sliderHeight);
    mBassKnob.setBounds(mBassKnobLayout.outLayout.x,
        mBassKnobLayout.outLayout.y,
        mBassKnobLayout.outLayout.sliderWidth,
        mBassKnobLayout.outLayout.sliderHeight);
    mMidKnob.setBounds(mMidKnobLayout.outLayout.x,
        mMidKnobLayout.outLayout.y,
        mMidKnobLayout.outLayout.sliderWidth,
        mMidKnobLayout.outLayout.sliderHeight);
    mHighKnob.setBounds(mHighKnobLayout.outLayout.x,
        mHighKnobLayout.outLayout.y,
        mHighKnobLayout.outLayout.sliderWidth,
        mHighKnobLayout.outLayout.sliderHeight);
    mOutputKnob.setBounds(mOutputKnobLayout.outLayout.x,
        mOutputKnobLayout.outLayout.y,
        mOutputKnobLayout.outLayout.sliderWidth,
        mOutputKnobLayout.outLayout.sliderHeight);
    mInputMeter.setBounds(mInputMeterLayout.outLayout.x,
        mInputMeterLayout.outLayout.y,
        mInputMeterLayout.outLayout.sliderWidth,
        mInputMeterLayout.outLayout.sliderHeight);
    mOutputMeter.setBounds(mOutputMeterLayout.outLayout.x,
        mOutputMeterLayout.outLayout.y,
        mOutputMeterLayout.outLayout.sliderWidth,
        mOutputMeterLayout.outLayout.sliderHeight);

    mIRButton.setBounds(mIRButtonLayout.outLayout.x,
        mIRButtonLayout.outLayout.y,
        mIRButtonLayout.outLayout.sliderWidth,
        mIRButtonLayout.outLayout.sliderHeight);

    mFileChooserButton.setBounds(mFileChooserButtonLayout.outLayout.x,
        mFileChooserButtonLayout.outLayout.y,
        mFileChooserButtonLayout.outLayout.sliderWidth,
        mFileChooserButtonLayout.outLayout.sliderHeight);
    mNAMChooserButton.setBounds(mNAMChooserButtonLayout.outLayout.x,
        mNAMChooserButtonLayout.outLayout.y,
        mNAMChooserButtonLayout.outLayout.sliderWidth,
        mNAMChooserButtonLayout.outLayout.sliderHeight);
    mNAMButton.setBounds(mNAMButtonLayout.outLayout.x,
        mNAMButtonLayout.outLayout.y,
        mNAMButtonLayout.outLayout.sliderWidth,
        mNAMButtonLayout.outLayout.sliderHeight);
    mIRVerbButton.setBounds(mIRVerbButtonLayout.outLayout.x,
        mIRVerbButtonLayout.outLayout.y,
        mIRVerbButtonLayout.outLayout.sliderWidth,
        mIRVerbButtonLayout.outLayout.sliderHeight);
    mVerbMixKnob.setBounds(mVerbMixKnobLayout.outLayout.x,
        mVerbMixKnobLayout.outLayout.y,
        mVerbMixKnobLayout.outLayout.sliderWidth,
        mVerbMixKnobLayout.outLayout.sliderHeight);
    mTopBar.setBounds(mTopBarLayout.outLayout.x,
        mTopBarLayout.outLayout.y,
        mTopBarLayout.outLayout.sliderWidth,
        mTopBarLayout.outLayout.sliderHeight);
    mVerbDropDown.setBounds(mVerbDropDownLayout.outLayout.x,
        mVerbDropDownLayout.outLayout.y,
        mVerbDropDownLayout.outLayout.sliderWidth,
        mVerbDropDownLayout.outLayout.sliderHeight);

    addAndMakeVisible(mInputMeter);
    addAndMakeVisible(mInputKnob);
    addAndMakeVisible(mGateKnob);
    addAndMakeVisible(mBassKnob);
    addAndMakeVisible(mMidKnob);
    addAndMakeVisible(mHighKnob);
    addAndMakeVisible(mOutputKnob);
    addAndMakeVisible(mOutputMeter);
    addAndMakeVisible(mIRButton);
    addAndMakeVisible(mFileChooserButton);
    addAndMakeVisible(mNAMChooserButton);
    addAndMakeVisible(mNAMButton);
    addAndMakeVisible(mIRVerbButton);
    addAndMakeVisible(mVerbMixKnob);
    addAndMakeVisible(mTopBar);
    addAndMakeVisible(mVerbDropDown);

    mInputKnob.setLookAndFeel(&mKnobLookAndFeel);
    mGateKnob.setLookAndFeel(&mKnobLookAndFeel);
    mBassKnob.setLookAndFeel(&mKnobLookAndFeel);
    mMidKnob.setLookAndFeel(&mKnobLookAndFeel);
    mHighKnob.setLookAndFeel(&mKnobLookAndFeel);
    mOutputKnob.setLookAndFeel(&mKnobLookAndFeel);
    mVerbMixKnob.setLookAndFeel(&mKnobLookAndFeel);
    mFileChooserButton.setLookAndFeel(&mDropDownLookAndFeel);
    mNAMChooserButton.setLookAndFeel(&mDropDownLookAndFeel);
    mVerbDropDown.setLookAndFeel(&mDropDownLookAndFeel);
}
