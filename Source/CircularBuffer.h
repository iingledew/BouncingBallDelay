#pragma once

#include <vector>

class CircularBuffer {
public:

	CircularBuffer(int lengthSamples) {
		bufferLength = lengthSamples;
		buffer = std::vector<float>(bufferLength, 0.0f);  // 0.0f was initially 0
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