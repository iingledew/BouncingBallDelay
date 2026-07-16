/*
  ==============================================================================

    Knob.h
    Created: 25 Jun 2026 7:18:43pm
    Author:  georg

  ==============================================================================
*/

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "NewLookAndFeel.h"

class Knob final : public juce::Component, 
                   private juce::Slider::Listener
{
public:
    Knob(juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::String suffix, KnobImgStrip knobImgStrip, bool showValue = true, float mouseSensMultiplier = 1);   // originally had float* parameter as an argument
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;
    void setValue(float newValue);
    void paint(juce::Graphics& g) override;
    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;

    juce::Slider sliderComponent;

private:
    juce::Label valueComponent;
    juce::Label nameComponent;
    KnobLookAndFeel lookAndFeel;
    juce::AudioProcessorValueTreeState::SliderAttachment sliderAttachment;

    juce::String suffix;
    juce::String name;

    int imgWidth;
    int imgHeight;
};

//private juce::MouseListener
