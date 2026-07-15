/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>

//==============================================================================
BouncingBallAudioProcessor::BouncingBallAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
}

BouncingBallAudioProcessor::~BouncingBallAudioProcessor()
{
}

//==============================================================================
const juce::String BouncingBallAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BouncingBallAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BouncingBallAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BouncingBallAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BouncingBallAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BouncingBallAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BouncingBallAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BouncingBallAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BouncingBallAudioProcessor::getProgramName (int index)
{
    return {};
}

void BouncingBallAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BouncingBallAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    // processParameters();
    getParameters();
    updateFilter();
    updateBall();
}

void BouncingBallAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BouncingBallAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BouncingBallAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    // update and process the parameters
    getParameters();
    checkForChangedParameters();   // check for changed parameters

    // finding how many delays to compute
    if (feedback == 0) { maxDelaysFromFeedback = 1; }
    else if (feedback == 1) { maxDelaysFromFeedback = 50; }
    else {
        maxDelaysFromFeedback = std::ceil(std::log(0.01) / std::log(feedback));    // 0.01 is the selected minimum level
    }
    maxDelays = std::min({ ball.delaysNumber, maxDelaysFromFeedback });
    
    // clears output channels without corresponding input channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // loops through input channels
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        // loops through samples in the channel's buffer
        for (int s = 0; s < buffer.getNumSamples(); s++) {
            float currentSample = channelData[s];

            float outputSample = (currentSample * (1 - mix)) + (processSample(currentSample, channel) * mix);

            channelData[s] = outputSample;
        }
    }
}

float BouncingBallAudioProcessor::processSample(float sample, int channel)  // should have a filterProcessFunc argument
{
    float wetSample = 0;
    float currentPan = pan;
    
    // loops through each delay
    for (int d = 0; d < maxDelays; d++) {
        if (ball.delays[d] != 0) { 
            delay.updateParams(ball.delays[d]); 
        }

        // sending to wet output
        float delayedSample = delay.readSampleFrac(0, channel);
        float feedbackSample = delayedSample;
        panSample(&delayedSample, currentPan, channel);
        currentPan = std::abs((panType == panTypes::PingPong) - currentPan);      // flips currentPan if PingPong mode is selected
        wetSample += delayedSample;

        // feedback path
        feedbackSample *= feedback;
        if (filterType != Filters::None) {
            filters[d].processSample(&feedbackSample, channel);      // all the issues happen when a filter is switched on, down to 5-6% CPU usage in release anyway
        }
        delay.setSampleAtReadPos(feedbackSample, channel);
    }

    delay.writeSample(sample, channel);

    return wetSample;
}

void BouncingBallAudioProcessor::updateBall() 
{
    delay.clear();
    ball.generateDelaysMath(BouncingBallAudioProcessor::getSampleRate(), speed, angle, gravity, damping, height);
}

void BouncingBallAudioProcessor::updateFilter() 
{
    if (filterType != Filters::None) {
        for (int i = 0; i < 50; i++) {
            filters[i].generateCoefficients(cutoff, resonance, filterType, getSampleRate());
        }
    }
}

void BouncingBallAudioProcessor::panSample(float* inputSample, float panValue, int channel) {
    // takes in pan value between 0 - 1
    // mult system means that in the centre both channels are 1 and at the sides each channel is only 1
    float pan[2] = {};
    float distanceFromCentre = std::abs(panValue - 0.5);
    float mult = 2*(1 - distanceFromCentre);

    pan[0] = (1 - panValue) * mult;
    pan[1] = panValue * mult;

    *inputSample *= pan[channel];
}

//==============================================================================
bool BouncingBallAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BouncingBallAudioProcessor::createEditor()
{
    return new BouncingBallAudioProcessorEditor (*this);
}

//==============================================================================
void BouncingBallAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto paramState = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(paramState.createXml());
    copyXmlToBinary(*xml, destData);
}

void BouncingBallAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr) {
        if (xmlState->hasTagName(apvts.state.getType())) {
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

float BouncingBallAudioProcessor::mapParameterValue(float inputValue, float inRangeMin, float inRangeMax, float outRangeMin, float outRangeMax) 
{
    float frac = (inputValue - inRangeMin) / (inRangeMax - inRangeMin);
    return ((outRangeMax - outRangeMin) * frac) + outRangeMin;
}

void BouncingBallAudioProcessor::getParameters() 
{
    oldResonance    = resonance;
    oldCutoff       = cutoff;
    oldFilterType   = filterType;

    oldAngle        = angle;
    oldSpeed        = speed;
    oldGravity      = gravity;
    oldDamping      = damping;
    oldHeight       = height;

    float pMix          = apvts.getRawParameterValue("mix")->load();              
    float pSpeed        = apvts.getRawParameterValue("speed")->load();           
    float pAngle        = apvts.getRawParameterValue("angle")->load();           
    float pHeight       = apvts.getRawParameterValue("height")->load();          
    float pDamping      = apvts.getRawParameterValue("damping")->load();     
    float pGravity      = apvts.getRawParameterValue("gravity")->load();        
    float pCutoff       = apvts.getRawParameterValue("cutoff")->load();         
    float pResonance    = apvts.getRawParameterValue("resonance")->load();   
    float pFilterType   = apvts.getRawParameterValue("filterType")->load();  
    float pFeedback     = apvts.getRawParameterValue("feedback")->load();       
    float pPan          = apvts.getRawParameterValue("pan")->load();
    float pPanType      = apvts.getRawParameterValue("panType")->load();

    mix         = mapParameterValue(pMix, 0.0, 100.0, 0.0, 1.0);
    feedback    = mapParameterValue(pFeedback, 0.0, 100.0, 0.0, 1.0);
    pan         = mapParameterValue(pPan, -100.0, 100.0, 0.0, 1.0);
    resonance   = mapParameterValue(pResonance, 0.0, 100.0, 0.05, 1.00);
    cutoff      = pCutoff;
    filterType  = static_cast<int>(pFilterType);
    panType     = static_cast<int>(pPanType);

    angle       = mapParameterValue(pAngle, -90.0, 90, PI, 0.0);
    speed       = mapParameterValue(pSpeed, 0.0, 100.0, 0.0, 500.0);       // max speed used to be 1000, reducing to 500
    gravity     = mapParameterValue(pGravity, 0.0, 100.0, 100.0, 2000.0);
    damping     = mapParameterValue(pDamping, 0.0, 100.0, 0.95, 0);
    height      = mapParameterValue(pHeight, 0.0, 100.0, 0.0, 500.0);
}

void BouncingBallAudioProcessor::checkForChangedParameters() 
{
    // filter
    if ((!juce::approximatelyEqual(cutoff, oldCutoff)) ||
        (!juce::approximatelyEqual(resonance, oldResonance)) ||
        (!juce::approximatelyEqual(filterType, oldFilterType)))
    {
        updateFilter();
    }

    // ball
    if ((!juce::approximatelyEqual(angle, oldAngle)) ||
        (!juce::approximatelyEqual(speed, oldSpeed)) ||
        (!juce::approximatelyEqual(gravity, oldGravity)) ||
        (!juce::approximatelyEqual(damping, oldDamping)) || 
        (!juce::approximatelyEqual(height, oldHeight)))
    {
        updateBall();
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout BouncingBallAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;    // vector containing multiple AudioParameters

    auto mix = std::make_unique<juce::AudioParameterFloat>(
        "mix", "Mix", 0.0f, 100.0f, 50.0f);
    auto speed = std::make_unique<juce::AudioParameterFloat>(
        "speed", "Speed", 0.0f, 100.0f, 10.0f);
    auto angle = std::make_unique<juce::AudioParameterFloat>(
        "angle", "Angle", -85.0f, 85.0f, 0.0f);
    auto height = std::make_unique<juce::AudioParameterFloat>(
        "height", "Height", 0.0f, 100.0f, 0.0f);
    auto damping = std::make_unique<juce::AudioParameterFloat>(
        "damping", "Damping", 0.0f, 100.0f, 0.0f);
    auto gravity = std::make_unique<juce::AudioParameterFloat>(
        "gravity", "Gravity", 0.0f, 100.0f, 50.0f);
    auto cutoff = std::make_unique<juce::AudioParameterFloat>(
        "cutoff", "Cutoff", juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.25f), 1000.0f);
    auto resonance = std::make_unique<juce::AudioParameterFloat>(
        "resonance", "Resonance", 0.0f, 100.0f, 50.0f);
    auto filterType = std::make_unique<juce::AudioParameterChoice>(
        "filterType", "Filter Type", juce::StringArray{ "None", "LPF", "HPF", "BPF"}, 0);
    auto feedback = std::make_unique<juce::AudioParameterFloat>(
        "feedback", "Amount", 0.0f, 100.0f, 50.0f);
    auto pan = std::make_unique<juce::AudioParameterFloat>(
        "pan", "Pan", -100.0f, 100.0f, 0.0f);
    auto panType = std::make_unique<juce::AudioParameterChoice>(
        "panType", "Pan Type", juce::StringArray {"Fixed", "PingPong"}, 0);

    params.add(std::move(mix));
    params.add(std::move(speed));
    params.add(std::move(angle));
    params.add(std::move(height));
    params.add(std::move(damping));
    params.add(std::move(gravity));
    params.add(std::move(cutoff));
    params.add(std::move(resonance));
    params.add(std::move(filterType));
    params.add(std::move(feedback));
    params.add(std::move(pan));
    params.add(std::move(panType));

    return params;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BouncingBallAudioProcessor();
}