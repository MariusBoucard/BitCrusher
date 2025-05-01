#pragma once
#include "Bones/ToneStack.h"
#include <JuceHeader.h>
#include "dsp/NoiseGate.h"

struct ParameterSetup
{
    recursive_linear_filter::BiquadParams mMidParams{ 44100., 1., 0., 0. };
}; 