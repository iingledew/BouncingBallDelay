/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

//==============================================================================
BouncingBallAudioProcessorEditor::BouncingBallAudioProcessorEditor (BouncingBallAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(mixKnob);
    addAndMakeVisible(speedKnob);
    addAndMakeVisible(angleKnob);
    //addAndMakeVisible(heightKnob);
    addAndMakeVisible(dampingKnob);
    addAndMakeVisible(gravityKnob);
    addAndMakeVisible(cutoffKnob);
    addAndMakeVisible(resonanceKnob);
    addAndMakeVisible(filterTypeKnob);
    addAndMakeVisible(feedbackKnob);
    addAndMakeVisible(panKnob);
    addAndMakeVisible(panTypeKnob);

    speedKnob.sliderComponent.onValueChange = [this]() {
        repaint();
    };
    angleKnob.sliderComponent.onValueChange = [this]() {
        repaint();
    };

    setSize (800, 400);
}

BouncingBallAudioProcessorEditor::~BouncingBallAudioProcessorEditor()
{
}

//==============================================================================
void BouncingBallAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::white);
    g.drawImageAt(Images::getBackgroundImg(), 0, 0);
    g.setColour(juce::Colours::black);
    
    // update the ball visualisation
    float startOffsetX = 25 * std::cos(audioProcessor.angle);
    float startOffsetY = 25 * std::sin(audioProcessor.angle);
    float deltaX = (audioProcessor.speed / 8) * std::cos(audioProcessor.angle);
    float deltaY = (audioProcessor.speed / 8) * std::sin(audioProcessor.angle);
    juce::Line<float> arrowLine = juce::Line<float>(400+startOffsetX, 250-startOffsetY, 400+startOffsetX+deltaX, 250-startOffsetY-deltaY);    // centre of the ball is around 400, 250
    g.drawArrow(arrowLine, 3, 10, 7);

    // set the font size and draw text to the screen
    g.setFont(15.0f);
}

void BouncingBallAudioProcessorEditor::resized()
{
    speedKnob.setCentrePosition(280, 320);    // originally started at 50
    angleKnob.setCentrePosition(360, 320);
    dampingKnob.setCentrePosition(440, 320);
    gravityKnob.setCentrePosition(520, 320);

    cutoffKnob.setCentrePosition(100, 320);    // 80 in the y and x direction
    resonanceKnob.setCentrePosition(180, 320);    // height of the knob is 80
    filterTypeKnob.setCentrePosition(140, 240);
    feedbackKnob.setCentrePosition(140, 160);

    panKnob.setCentrePosition(660, 320);
    mixKnob.setCentrePosition(660, 160);
    panTypeKnob.setCentrePosition(660, 240);

    // put the box in the centre
    // knobs should be centre +/- 40
}

/*
void BouncingBallAudioProcessorEditor::mouseDrag(const juce::MouseEvent&)     // scroll as well
{
    repaint();
}

void BouncingBallAudioProcessorEditor::mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails&)
{
    repaint();
}
*/