/*
  ==============================================================================

    Filters.h
    Created: 30 Jun 2026 1:38:42pm
    Author:  georg

  ==============================================================================
*/

#pragma once
#include "CircularBuffer.h"
#include <string>
#include <cmath>

#define PI 3.14159

namespace Filters 
{
    enum filterTypes {
        None, LPF, HPF, BPF
    };

    class SecondOrderBiquad {
    public:
        void processSample(float* inputSample, int channel);
        void generateCoefficientsLPF(float filterFrequency, float filterResonance, int sampleRate);
        void generateCoefficientsHPF(float filterFrequency, float filterResonance, int sampleRate);
        void generateCoefficientsBPF(float filterFrequency, float filterResonance, int sampleRate);
        void generateCoefficients(float filterFrequency, float filterResonance, int filterType, int sampleRate);

    private:
        std::vector<CircularBuffer> xBuffers = std::vector<CircularBuffer>(2, CircularBuffer(2));      // length 2 circular buffer for each channel
        std::vector<CircularBuffer> yBuffers = std::vector<CircularBuffer>(2, CircularBuffer(2));      // length 2 circular buffer for each channel

        float a[3] = {};   // coefficients
        float b[3] = {};
        float a0_reciprocal;
    };
}