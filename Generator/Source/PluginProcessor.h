/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Rectifier.h"
#include "LimiterDriver.h"
#include "Generator.h"

//==============================================================================
/**
*/
class GeneratorAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    GeneratorAudioProcessor();
    ~GeneratorAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    AudioBuffer<float> controlBuffer;

    Rectifier rect[2];
    LimiterDriver limiterDriver[2];
    Generator generator[2];

    dsp::Oversampling<float> oversampling = dsp::Oversampling<float> (2, 2, dsp::Oversampling<float>::filterHalfBandPolyphaseIIR);
    const int osMult = 4;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GeneratorAudioProcessor)
};
