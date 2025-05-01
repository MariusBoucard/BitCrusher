

void RootViewComponent::defineKnobLayout()
{
    mTopBarLayout.inLayout.x = 0;
    mTopBarLayout.inLayout.y = 0;
    mTopBarLayout.inLayout.frameWidth = getLocalBounds().getWidth();
    mTopBarLayout.inLayout.frameHeight = 30;
    mTopBarLayout.inLayout.ratio = 1.f;
    mTopBarLayout.inLayout.textboxHeight = 0;
    mTopBarLayout.inLayout.textboxPadding = 0;

    mLeftSectionLayout.inLayout.x = 50;
    mLeftSectionLayout.inLayout.y = 500;
	mLeftSectionLayout.inLayout.frameWidth = 390;
    mLeftSectionLayout.inLayout.frameHeight = 400;
	mLeftSectionLayout.inLayout.ratio = 1.f;
    mLeftSectionLayout.inLayout.textboxHeight = 0;
    mLeftSectionLayout.inLayout.textboxPadding = 0;

    mMidSectionLayout.inLayout.x = 500;
    mMidSectionLayout.inLayout.y = 200;
	mMidSectionLayout.inLayout.frameWidth = 900;
    mMidSectionLayout.inLayout.frameHeight = 1000;
	mMidSectionLayout.inLayout.ratio = 1.f;

    mRightSectionLayout.inLayout.x = 1400;
    mRightSectionLayout.inLayout.y = 200;
    mRightSectionLayout.inLayout.frameWidth = 400;
    mRightSectionLayout.inLayout.frameHeight = 500;
    mRightSectionLayout.inLayout.ratio = 1.f;

  

    computeKnobLayout(mLeftSectionLayout);
    computeKnobLayout(mTopBarLayout);
    computeKnobLayout(mMidSectionLayout);
	computeKnobLayout(mRightSectionLayout);
}


void RootViewComponent::configureNodes(juce::AudioProcessor& inProcessor)
{
    auto bounds = getLocalBounds();

    setSliderAttachement(inProcessor);
   
    
    mLeftSection.setBounds(mLeftSectionLayout.outLayout.x,
		mLeftSectionLayout.outLayout.y,
		mLeftSectionLayout.outLayout.sliderWidth,
		mLeftSectionLayout.outLayout.sliderHeight);
    

    mTopBar.setBounds(mTopBarLayout.outLayout.x,
        mTopBarLayout.outLayout.y,
        mTopBarLayout.outLayout.sliderWidth,
        mTopBarLayout.outLayout.sliderHeight);
    
mMidSection.setBounds(mMidSectionLayout.outLayout.x,
mMidSectionLayout.outLayout.y,
mMidSectionLayout.outLayout.sliderWidth,
mMidSectionLayout.outLayout.sliderHeight);

	mRightSection.setBounds(mRightSectionLayout.outLayout.x,
        		mRightSectionLayout.outLayout.y,
        		mRightSectionLayout.outLayout.sliderWidth,
        		mRightSectionLayout.outLayout.sliderHeight);
	
	addAndMakeVisible(mRightSection);
	
	addAndMakeVisible(mTopBar);

addAndMakeVisible(mMidSection);

    
    addAndMakeVisible(mLeftSection);
    
    addAndMakeVisible(mTopBar);

   
}
