#pragma once

#include "CircularBuffer.h"

int CircularBuffer::getReadPos(int delaySamples) {
	//int readPos = writePos - delaySamples;
	//if (readPos < 0) { readPos += bufferLength; }

	//new
	int readPos = (writePos - delaySamples) & (bufferLength - 1);

	return readPos;
}

float CircularBuffer::readValue(int delaySamples) {
	//int readPos = writePos - delaySamples;
	//if (readPos < 0) { readPos += bufferLength; }

	//return buffer[readPos];

	// new
	return buffer[getReadPos(delaySamples)];
}

void CircularBuffer::writeValue(float valueToWrite) {
	//writePos += 1;
	//if (writePos == bufferLength) { writePos = 0; }

	//new
	writePos = (writePos + 1) & (bufferLength - 1);

	buffer[writePos] = valueToWrite;
}

void CircularBuffer::setValueAtReadPos(float newValue, int delaySamples) {
	buffer[getReadPos(delaySamples)] = newValue;
}

void CircularBuffer::clear() {
	std::fill(buffer.begin(), buffer.end(), 0);
}