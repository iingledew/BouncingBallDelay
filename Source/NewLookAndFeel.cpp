/*
  ==============================================================================

    NewLookAndFeel.cpp
    Created: 24 Jun 2026 7:18:52pm
    Author:  georg

  ==============================================================================
*/

#include "NewLookAndFeel.h"

KnobLookAndFeel::KnobLookAndFeel(juce::Image imgStrip, int numFrames)
    : imgStrip(imgStrip), numFrames(numFrames)
{

}

void KnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& s)
{
    int imgWidth = imgStrip.getWidth();
    int imgHeight = imgStrip.getHeight() / numFrames;

    int frameIndex = static_cast<int>(sliderPosProportional * static_cast<float>(numFrames - 1));
    int frameYVal = imgHeight * frameIndex;

    g.drawImage(imgStrip, 0, 0, imgWidth, imgHeight, 0, frameYVal, imgWidth, imgHeight);

    //g.drawText(static_cast<juce::String>(s.getValue()), 0, 0, imgWidth, imgHeight, juce::Justification::centred);
}