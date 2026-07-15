/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Delay.h"
#include "Filters.h"
#include "Ball.h"

#define PI 3.14159

//==============================================================================
/**
*/
class BouncingBallAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    BouncingBallAudioProcessor();
    ~BouncingBallAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    float processSample(float sample, int channel);

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    void updateBall();
    void updateFilter();
    //void processParameters();
    void getParameters();
    void checkForChangedParameters();
    float mapParameterValue(float inputValue, float inRangeMin, float inRangeMax, float outRangeMin, float outRangeMax);
    void panSample(float* inputSample, float panValue, int channel);

    //==============================================================================
    juce::AudioProcessorValueTreeState apvts;

    // main parameters
    float mix;
    float feedback;  // 0.8
    float pan;       // 0.5
    int panType;

    // filter parameters
    float cutoff;   // 1000
    float resonance; // 0.5
    int filterType;    // 0
    // old
    float oldCutoff = -100;
    float oldResonance = -100;
    int oldFilterType = -100;

    // ball parameters
    float speed;
    float angle;
    float damping;
    float gravity;
    float height;
    // old
    float oldSpeed = -100;
    float oldAngle = -100;
    float oldDamping = -100;
    float oldGravity = -100;
    float oldHeight = -100;

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    Delay delay;
    Ball ball;
    Filters::SecondOrderBiquad filters[50];

    int maxDelaysFromFeedback = 50;
    int maxDelays = 50;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BouncingBallAudioProcessor)
};

enum panTypes {
    Fixed, PingPong
};