/*
  ==============================================================================

    Knob.cpp
    Created: 25 Jun 2026 7:18:43pm
    Author:  georg

  ==============================================================================
*/

#include "Knob.h"

Knob::Knob(juce::AudioProcessorValueTreeState& apvts, juce::String paramID, juce::String suffix, KnobImgStrip knobImgStrip, bool showValue, float mouseSensMultiplier) :  sliderAttachment(apvts, paramID, sliderComponent),
                                                                                                                                                                          valueComponent("value", "0"),   
                                                                                                                                                                          suffix(suffix),
                                                                                                                                                                          name(apvts.getParameter(paramID)->name),
                                                                                                                                                                          nameComponent("name", "name"),
                                                                                                                                                                          lookAndFeel(knobImgStrip.getImg(), knobImgStrip.getNumFrames())
{
    nameComponent.setJustificationType(juce::Justification::centred);
    nameComponent.setColour(juce::Label::textColourId, juce::Colours::black);
    nameComponent.setText(name, juce::NotificationType::dontSendNotification);

    valueComponent.setJustificationType(juce::Justification::centred);
    valueComponent.setColour(juce::Label::textColourId, juce::Colours::black);
    valueComponent.setAlpha(0.5);
    //valueComponent.setFont(juce::Font::Font(10.0f, juce::Font::plain));        deprecated?
    valueComponent.setFont(juce::Font(juce::FontOptions (10.0f, juce::Font::plain)));
    valueComponent.setText(static_cast<juce::String>(int(sliderComponent.getValue()))+suffix, juce::NotificationType::dontSendNotification);

    sliderComponent.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    sliderComponent.setMouseDragSensitivity(sliderComponent.getMouseDragSensitivity() / mouseSensMultiplier);
    sliderComponent.setLookAndFeel(&lookAndFeel);
    sliderComponent.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 20);
    sliderComponent.addListener(this);
    sliderComponent.addMouseListener(this, true);

    if (showValue) { addAndMakeVisible(valueComponent); }
    addAndMakeVisible(sliderComponent);
    addAndMakeVisible(nameComponent);

    imgWidth = lookAndFeel.imgStrip.getWidth();
    imgHeight = lookAndFeel.imgStrip.getHeight() / lookAndFeel.numFrames;
    setSize(imgWidth, imgHeight + imgHeight/4);
}

void Knob::mouseEnter(const juce::MouseEvent& event) 
{
    valueComponent.setAlpha(1);
}

void Knob::mouseExit(const juce::MouseEvent& event)
{
    valueComponent.setAlpha(0.5);
}

void Knob::paint(juce::Graphics& g)
{

}

void Knob::resized() 
{
    sliderComponent.setBounds(0, 0, getWidth(), getHeight());
    valueComponent.setSize(imgWidth/2, imgHeight);
    valueComponent.setCentrePosition(imgWidth/2, imgHeight/2);
    nameComponent.setSize(imgWidth, imgHeight/4);
    nameComponent.setTopLeftPosition(0, imgHeight);
}

void Knob::sliderValueChanged(juce::Slider* slider) 
{
    valueComponent.setText(static_cast<juce::String>(int(sliderComponent.getValue()))+suffix, juce::NotificationType::dontSendNotification);
}

void Knob::setValue(float newValue) 
{
    sliderComponent.setValue(newValue);
}
