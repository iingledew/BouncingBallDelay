/*
  ==============================================================================

    Filters.cpp
    Created: 30 Jun 2026 1:38:42pm
    Author:  georg

  ==============================================================================
*/

#include "Filters.h"

void Filters::SecondOrderBiquad::processSample(float* inputSample, int channel)
{
    // processing
    float outputSample = *inputSample * b[0];

    for (int i = 0; i < 2; i++) {
        outputSample += xBuffers[channel].readValue(i) * b[i + 1];
        outputSample -= yBuffers[channel].readValue(i) * a[i + 1];
    }

    outputSample *= a0_reciprocal;

    xBuffers[channel].writeValue(*inputSample);
    yBuffers[channel].writeValue(outputSample);

    *inputSample = outputSample;
}

void Filters::SecondOrderBiquad::generateCoefficients(float filterFrequency, float filterResonance, int filterType, int sampleRate)
{
    switch (filterType) {
    case LPF:
        generateCoefficientsLPF(filterFrequency, filterResonance, sampleRate);
        break;

    case HPF:
        generateCoefficientsHPF(filterFrequency, filterResonance, sampleRate);
        break;

    case BPF:
        generateCoefficientsBPF(filterFrequency, filterResonance, sampleRate);
    }

    a0_reciprocal = 1 / a[0];
}

void Filters::SecondOrderBiquad::generateCoefficientsLPF(float filterFrequency, float filterResonance, int sampleRate)
{
    // coefficient calculation
    float Q = filterResonance;
    float w0 = 2 * PI * (filterFrequency / sampleRate);
    float cos_w0 = std::cos(w0);
    float sin_w0 = std::sin(w0);
    float alpha = sin_w0 / (2*Q);

    b[0] = (1 - cos_w0) / 2;
    b[1] = 1 - cos_w0;
    b[2] = b[0];

    a[0] = 1 + alpha;
    a[1] = -2 * cos_w0;
    a[2] = 1 - alpha;
}

void Filters::SecondOrderBiquad::generateCoefficientsHPF(float filterFrequency, float filterResonance, int sampleRate) {
    // coefficient calculation
    float Q = filterResonance;
    float w0 = 2 * PI * (filterFrequency / sampleRate);
    float cos_w0 = std::cos(w0);
    float sin_w0 = std::sin(w0);
    float alpha = sin_w0 / (2 * Q);

    b[0] = (1 + cos_w0) / 2;
    b[1] = -1 - cos_w0;
    b[2] = b[0];

    a[0] = 1 + alpha;
    a[1] = -2 * cos_w0;
    a[2] = 1 - alpha;
}

void Filters::SecondOrderBiquad::generateCoefficientsBPF(float filterFrequency, float filterResonance, int sampleRate) {
    // coefficient calculation
    // previous implementation had constant skirt gain but that means almost no output with low Q
    float Q = filterResonance;
    float w0 = 2 * PI * (filterFrequency / sampleRate);
    float cos_w0 = std::cos(w0);
    float sin_w0 = std::sin(w0);
    float alpha = sin_w0 / (2 * Q);

    //b[0] = sin_w0 / 2;     constant skirt gain
    b[0] = alpha;
    b[1] = 0;
    b[2] = -b[0];

    a[0] = 1 + alpha;
    a[1] = -2 * cos_w0;
    a[2] = 1 - alpha;
}

