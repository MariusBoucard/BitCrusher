#include <JuceHeader.h>
#include "Bones/ToneStack.h"


class Mappers
{
private:
	Mappers()
		: mSampleRate(44100.0) {} 
	Mappers(const Mappers&) = delete; 
	Mappers& operator=(const Mappers&) = delete;

public:
	static Mappers& getMapperInstance()
	{
		static Mappers instance;
		return instance;
	}

	static void setSampleRate(double sampleRate)
	{
		getMapperInstance().mSampleRate = sampleRate;
	}


	static void setToneStackMid(recursive_linear_filter::BiquadParams& inMidSetup, const double val) {

		const double sampleRate = getMapperInstance().mSampleRate;
		const double midGainDB = 10 * (val - 0.5); // +/- 10
		const double midFrequency = 425.0;
		const double midQuality = midGainDB < 0.0 ? 1.5 : 0.7;
		recursive_linear_filter::BiquadParams midParams(sampleRate, midFrequency, midQuality, midGainDB);
		inMidSetup = midParams;
	}

private:
	double mSampleRate;
};
