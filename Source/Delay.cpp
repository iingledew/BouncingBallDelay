#pragma once

#include "Delay.h"

void Delay::processSampleFrac(float* inputSample, float lfoShift, int channel) {
	float inSample = *inputSample;

	float totalDelay = delaySamples + lfoShift;
	int upperDelay = ceil(totalDelay);
	int lowerDelay = upperDelay - 1;
	float fraction = totalDelay - lowerDelay;

	float upperDelaySample = delayBuffers[channel].readValue(upperDelay);
	float lowerDelaySample = delayBuffers[channel].readValue(lowerDelay);
	float diff = upperDelaySample - lowerDelaySample;

	float delayedSample = lowerDelaySample + (fraction * diff);

	delayBuffers[channel].writeValue(inSample);    // inSample + fb*delayedSample

	*inputSample = delayedSample;
}

float Delay::readSampleFrac(float lfoShift, int channel) {
	float totalDelay = delaySamples + lfoShift;
	int upperDelay = ceil(totalDelay);
	int lowerDelay = upperDelay - 1;
	float fraction = totalDelay - lowerDelay;

	float upperDelaySample = delayBuffers[channel].readValue(upperDelay);
	float lowerDelaySample = delayBuffers[channel].readValue(lowerDelay);
	float diff = upperDelaySample - lowerDelaySample;

	float delayedSample = lowerDelaySample + (fraction * diff);

	return delayedSample;
}

void Delay::writeSample(float inputSample, int channel) {
	delayBuffers[channel].writeValue(inputSample);
}

void Delay::setSampleAtReadPos(float newValue, int channel) {
	delayBuffers[channel].setValueAtReadPos(newValue, delaySamples);
}

void Delay::clear() {
	for (int channel = 0; channel < 2; channel++) {
		delayBuffers[channel].clear();
	}
}

void Delay::updateParams(int newDelaySamples) {
	delaySamples = newDelaySamples;
}