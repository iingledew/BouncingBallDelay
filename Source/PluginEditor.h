/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "NewLookAndFeel.h"
#include "Knob.h"

//==============================================================================
/**
*/
class BouncingBallAudioProcessorEditor  : public juce::AudioProcessorEditor

{
public:
    BouncingBallAudioProcessorEditor (BouncingBallAudioProcessor&);
    ~BouncingBallAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    BouncingBallAudioProcessor& audioProcessor;

    Knob mixKnob{ audioProcessor.apvts, "mix", "%", Images::standardKnob};
    Knob speedKnob{ audioProcessor.apvts, "speed", "%", Images::standardKnob};
    Knob angleKnob{ audioProcessor.apvts, "angle", "dgs", Images::standardKnob};
    Knob heightKnob{ audioProcessor.apvts, "height", "%", Images::standardKnob};
    Knob dampingKnob{ audioProcessor.apvts, "damping", "%", Images::standardKnob};
    Knob gravityKnob{ audioProcessor.apvts, "gravity", "%", Images::standardKnob};
    Knob cutoffKnob{ audioProcessor.apvts, "cutoff", "Hz", Images::standardKnob};
    Knob resonanceKnob{ audioProcessor.apvts, "resonance", "%", Images::standardKnob};
    Knob filterTypeKnob{ audioProcessor.apvts, "filterType", "", Images::optionSlider, false, 2};  // could have a seperate method for changing mouse drag sensitivity
    Knob feedbackKnob{ audioProcessor.apvts, "feedback", "%", Images::standardKnob};
    Knob panKnob{ audioProcessor.apvts, "pan", "%", Images::standardKnob};
    Knob panTypeKnob{ audioProcessor.apvts, "panType", "", Images::optionSlider, false, 2};

    int testCounter = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BouncingBallAudioProcessorEditor)
};
