/*
  ==============================================================================

    Ball.cpp
    Created: 14 Jun 2026 6:17:55pm
    Author:  georg

  ==============================================================================
*/

#include "Ball.h"
#include <cmath>
#include <algorithm>

void Ball::generateDelays(double sampleRate, float xInit[2], float vInit[2], float dampingInit[2], float gInit)
{
    active = true;
    delaysNumber = 0;
    for (int dimension = 0; dimension < 2; dimension++) {
        x[dimension] = xInit[dimension];
        v[dimension] = vInit[dimension];
        damping[dimension] = dampingInit[dimension];
    }
    a[1] = gInit * -1;

    float dt = 1 / sampleRate;
    int sampleCount = 0;

    while (active) {

        for (int dimension = 0; dimension < 2; dimension++) {
            v[dimension] += a[dimension] * dt;
            x[dimension] += v[dimension] * dt;

            if ((x[dimension] >= size[dimension]) || (x[dimension] <= 0)) {
                delays[delaysNumber] = sampleCount;
                delaysNumber++;

                v[dimension] *= (damping[dimension] * -1);

                if (x[dimension] >= size[dimension]) {
                    x[dimension] = size[dimension];
                }
                else {
                    x[dimension] = 0;
                    if (std::abs(v[1]) < 0.3) {
                        active = false;
                    }
                }

            }
        }

        sampleCount++;
    }
}

//void Ball::generateDelaysMath(double sampleRate, float xInit[2], float vInit[2], float dampingInit[2], float gInit)
void Ball::generateDelaysMath(double sampleRate, float speed, float angle, float gravity, float _damping, float height)    // xInit, vInit
{
    // clear delays
    delaysNumber = 0;
    std::fill_n(delays, 50, 0);

    // generate init values
    float xInit[2] = { 250, height };
    float vInit[2] = { (speed * std::cos(angle)), (speed * std::sin(angle)) };
    float dampingInit[2] = { _damping, _damping };

    // reset position and velocities
    for (int dimension = 0; dimension < 2; dimension++) {
        x[dimension] = xInit[dimension];
        v[dimension] = vInit[dimension];
        damping[dimension] = dampingInit[dimension];
    }
    //a[1] = gInit * -1;
    a[1] = gravity * -1;

    // local variables
    float currentTime = 0;
    float timeToWall[2] = {};
    float closestWallPosition[2] = {};

    for (int i = 0; i < 50; i++) {

        // y dimension
        float distanceToCeiling = size[1] - x[1];
        float distanceToFloor = size[1] - distanceToCeiling;

        if (v[1] > 0) {   // going upwards
            float peakUpwardsDisplacement = std::pow(v[1], 2) / (-2 * a[1]);
            float peakUpwardsHeight = x[1] + peakUpwardsDisplacement;

            if (peakUpwardsDisplacement < distanceToCeiling) {  // not reaching ceiling
                closestWallPosition[1] = 0;
                timeToWall[1] = v[1] / std::abs(a[1]);
                timeToWall[1] += std::sqrt((2 * peakUpwardsHeight) / std::abs(a[1]));
            }
            else {                                              // reaching ceiling
                closestWallPosition[1] = size[1];
                timeToWall[1] = ((-1 * v[1]) + std::sqrt(std::pow(v[1], 2) + 2 * std::abs(a[1]) * distanceToCeiling)) / std::abs(a[1]);
            }
        }

        else {           // going downwards
            closestWallPosition[1] = 0;
            timeToWall[1] = (v[1] + std::sqrt(std::pow(std::abs(v[1]), 2) + 2 * std::abs(a[1]) * distanceToFloor)) / std::abs(a[1]);    // distanceToFloor was distanceToCeiling
        }


        // x dimension
        float distanceToWall = 0;

        if (v[0] > 0) {  // going right
            closestWallPosition[0] = size[0];
            distanceToWall = size[0] - x[0];
        }
        else {           // going left or not moving
            closestWallPosition[0] = 0;
            distanceToWall = x[0];
        }

        if (v[0] == 0) { // not moving in x dimension
            timeToWall[0] = 99999999999;
        }
        else {
            timeToWall[0] = distanceToWall / std::abs(v[0]);
        }


        // calculation
        int closestDimension = (timeToWall[1] < timeToWall[0]);   // find dimension with shortest time
        float dt = timeToWall[closestDimension];
        
        for (int dimension = 0; dimension < 2; dimension++) {     // calculate new positions and velocities
            x[dimension] += (v[dimension] * dt) + (0.5 * a[dimension] * std::pow(dt, 2));
            v[dimension] += a[dimension] * dt;
        }

        x[closestDimension] = closestWallPosition[closestDimension];
        v[closestDimension] *= damping[closestDimension] * -1;                              // v is increasing massively each time

        currentTime += dt;

        if ((currentTime * sampleRate) > 900000){                                            // checks if past max delay length
            break;
        }

        if (closestDimension == 1 && closestWallPosition[1] == 0 && std::abs(v[1]) < 0.1) {  // check if ball on floor and not moving, may check for time here as well
            break;
        }

        delays[i] = static_cast<int>(sampleRate * currentTime);
        delaysNumber++;
    }
}