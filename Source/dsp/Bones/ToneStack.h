#pragma once

#include <string>
#include "dsp/dsp.h"
#include "dsp/RecursiveLinearFilter.h"
#include "../ParameterSetup.h"
namespace dsp
{
namespace tone_stack
{
class AbstractToneStack
{
public:

    virtual DSP_SAMPLE** Process(DSP_SAMPLE** inputs, const int numChannels, const int numFrames) = 0;
  virtual void Reset(const double sampleRate, const int maxBlockSize)
  {
    mSampleRate = sampleRate;
    mMaxBlockSize = maxBlockSize;
  };
  virtual void SetParam(const std::string name, const double val) = 0;

protected:
  double GetSampleRate() const { return mSampleRate; };
  double mSampleRate = 0.0;
  int mMaxBlockSize = 0;
};

class BasicNamToneStack : public AbstractToneStack
{
public:
    BasicNamToneStack(ParameterSetup& inParameterSetup)
        : mParameterSetup(inParameterSetup)
        , mBassVal(0.5)
        , mMiddleVal(0.5)
        , mTrebleVal(0.5)
    {

  };

  ~BasicNamToneStack() = default;

  DSP_SAMPLE** Process(DSP_SAMPLE** inputs, const int numChannels, const int numFrames);
  virtual void Reset(const double sampleRate, const int maxBlockSize) override;
  // :param val: Assumed to be between 0 and 10, 5 is "noon"
  virtual void SetParam(const std::string name, const double val) override;


protected:
  recursive_linear_filter::LowShelf mToneBass;
  recursive_linear_filter::Peaking mToneMid;
  recursive_linear_filter::HighShelf mToneTreble;

  ParameterSetup& mParameterSetup;
  double mBassVal ;
  double mMiddleVal;
  double mTrebleVal;
};
}; // namespace tone_stack
}; // namespace dsp
