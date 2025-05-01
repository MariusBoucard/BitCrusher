

void RootViewComponent::defineKnobLayout()
{
    mTopBarLayout.inLayout.x = 0;
    mTopBarLayout.inLayout.y = 0;
    mTopBarLayout.inLayout.frameWidth = getLocalBounds().getWidth();
    mTopBarLayout.inLayout.frameHeight = 30;
    mTopBarLayout.inLayout.ratio = 1.f;
    mTopBarLayout.inLayout.textboxHeight = 0;
    mTopBarLayout.inLayout.textboxPadding = 0;

    mMidKnobLayout.inLayout.x = 270;
    mMidKnobLayout.inLayout.y = 504;
    mMidKnobLayout.inLayout.frameWidth = 173;
    mMidKnobLayout.inLayout.frameHeight = 181;
    mMidKnobLayout.inLayout.ratio = 1.f;
    mMidKnobLayout.inLayout.textboxHeight = 0;
    mMidKnobLayout.inLayout.textboxPadding = 0;

    computeKnobLayout(mMidKnobLayout);
    computeKnobLayout(mTopBarLayout);
}


void RootViewComponent::configureNodes(juce::AudioProcessor& inProcessor)
{
    auto bounds = getLocalBounds();
    mMidKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
   

    setSliderAttachement(inProcessor);
   
    mMidKnob.setBounds(mMidKnobLayout.outLayout.x,
        mMidKnobLayout.outLayout.y,
        mMidKnobLayout.outLayout.sliderWidth,
        mMidKnobLayout.outLayout.sliderHeight);
  
    mTopBar.setBounds(mTopBarLayout.outLayout.x,
        mTopBarLayout.outLayout.y,
        mTopBarLayout.outLayout.sliderWidth,
        mTopBarLayout.outLayout.sliderHeight);
    
    mMidKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    
    addAndMakeVisible(mMidKnob);
    
    addAndMakeVisible(mTopBar);

    mMidKnob.setLookAndFeel(&mKnobLookAndFeel);
   
}
