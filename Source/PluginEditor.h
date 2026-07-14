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
                                          //private juce::MouseListener
{
public:
    BouncingBallAudioProcessorEditor (BouncingBallAudioProcessor&);
    ~BouncingBallAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //void mouseDrag(const juce::MouseEvent&) override;
    //void mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails&) override;

    BouncingBallAudioProcessor& audioProcessor;

    Knob mixKnob{ audioProcessor.apvts, "mix", "%", Images::standardKnob, true};
    Knob speedKnob{ audioProcessor.apvts, "speed", "%", Images::standardKnob, true };
    Knob angleKnob{ audioProcessor.apvts, "angle", "dgs", Images::standardKnob, true };
    Knob heightKnob{ audioProcessor.apvts, "height", "%", Images::standardKnob, true };
    Knob dampingKnob{ audioProcessor.apvts, "damping", "%", Images::standardKnob, true };
    Knob gravityKnob{ audioProcessor.apvts, "gravity", "%", Images::standardKnob, true };
    Knob cutoffKnob{ audioProcessor.apvts, "cutoff", "Hz", Images::standardKnob, true };
    Knob resonanceKnob{ audioProcessor.apvts, "resonance", "%", Images::standardKnob, true };
    Knob filterTypeKnob{ audioProcessor.apvts, "filterType", "", Images::optionSlider, false };
    Knob feedbackKnob{ audioProcessor.apvts, "feedback", "%", Images::standardKnob, true };
    Knob panKnob{ audioProcessor.apvts, "pan", "%", Images::standardKnob, true };
    Knob panTypeKnob{ audioProcessor.apvts, "panType", "", Images::optionSlider, false };

    int testCounter = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BouncingBallAudioProcessorEditor)
};
