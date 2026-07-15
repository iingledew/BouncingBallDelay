#pragma once

#include "CircularBuffer.h"

class Delay {
public:
	Delay() {
		//delayBuffers = std::vector<CircularBuffer>(2, CircularBuffer(1000000));   // initializes a vector of 2 buffers, each with 10000 as the constructor parameter
		delayBuffers = std::vector<CircularBuffer>(2, CircularBuffer(1000000));     // 1048576
	}

	void processSampleFrac(float* inputSample, float lfoShift, int channel);
	float readSampleFrac(float lfoShift, int channel);
	void writeSample(float inputSample, int channel);
	void setSampleAtReadPos(float newValue, int channel);
	void clear();
	void updateParams(int newDelaySamples);

	int delaySamples = 0;

private:
	std::vector<CircularBuffer> delayBuffers;
};