/*
  ==============================================================================

    Ball.h
    Created: 14 Jun 2026 6:18:05pm
    Author:  georg

  ==============================================================================
*/

#pragma once

class Ball {
public:

    /*
	Ball(float xInit[2], float vInit[2], float dampingInit[2], float gInit) {        // could pass init values to the constructor
        //generateDelays(44100, xInit, vInit, dampingInit, gInit);
        //generateDelaysMath(44100, xInit, vInit, dampingInit, gInit);
        generateDelaysMath(44100, speed, angle, gravity, _damping, height);
    }
    */
    Ball() {}

    void generateDelays(double sampleRate, float xInit[2], float vInit[2], float dampingInit[2], float gInit);
    void generateDelaysMath(double sampleRate, float speed, float angle, float gravity, float _damping, float height);

    int delays[50] = {};
    float volumes[50] = {};
    int delaysNumber = 0;

private:
    //float x[2] = { 250, 250 };
    //float v[2] = { 0, 0 };             // v[0] = 100 sounded cool
    //float a[2] = { 0, -981 };          // 1m = 100
    //float damping[2] = { 0.5, 0.8 };
    float x[2] = {};
    float v[2] = {};
    float a[2] = {};
    float damping[2] = {};
    
    float size[2] = { 500, 500 };

    bool active = true;
};