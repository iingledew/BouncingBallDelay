#pragma once

#include <vector>
#include <cmath>

#define ln2 0.69314718056

class CircularBuffer {
public:

	CircularBuffer(int lengthSamples) {
		// creates a vector where the length is the next power of two above the provided buffer length
		float exponent = std::ceil(std::log(lengthSamples) / ln2);
		bufferLength = std::pow(2, exponent);
		buffer = std::vector<float>(bufferLength, 0.0f);
	}

	int getReadPos(int delaySamples);
	float readValue(int delaySamples);
	void writeValue(float valueToWrite);
	void setValueAtReadPos(float newValue, int delaySamples);
	void clear();

private:
	std::vector<float> buffer;
	int writePos = 0;
	int bufferLength;
};