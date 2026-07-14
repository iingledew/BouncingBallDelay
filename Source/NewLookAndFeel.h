/*
  ==============================================================================

    NewLookAndFeel.h
    Created: 24 Jun 2026 7:18:26pm
    Author:  georg

  ==============================================================================
*/

#pragma once

#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <BinaryData.h>

class KnobImgStrip {
public:
    KnobImgStrip(const char* name, const int size, int numFrames) : name(name), size(size), numFrames(numFrames) {}

    juce::Image getImg() {
        return juce::ImageCache::getFromMemory(name, size);
    }

    int getNumFrames() {
        return numFrames;
    }

private:
    const char* name;
    const int size;
    int numFrames;
};

namespace Images
{
    inline juce::Image getBackgroundImg() 
    {
        return juce::ImageCache::getFromMemory(BinaryData::bouncing_ball_gui_v2_png, BinaryData::bouncing_ball_gui_v2_pngSize);
    }

    inline KnobImgStrip standardKnob(BinaryData::badly_drawn_3_bold_png, BinaryData::badly_drawn_3_bold_pngSize, 31);
    inline KnobImgStrip optionSlider(BinaryData::option_slider_v3_png, BinaryData::option_slider_v3_pngSize, 31);
}

class KnobLookAndFeel : public juce::LookAndFeel_V4 
{
public:
    KnobLookAndFeel(juce::Image imgStrip, int numFrames);
    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider&) override;

    juce::Image imgStrip;
    int numFrames;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KnobLookAndFeel)
};