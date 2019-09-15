/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../Drive_Amp/Source/DriveAmp.h"
#include "../../Filter/Source/Filter.h"
#include "GeneratorProcessor.h"
#include "MixCircuit.h"
#include "SummingAmp.h"
#include "OutputFilter.h"

//==============================================================================
/**
*/
class Aphex_exciterAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Aphex_exciterAudioProcessor();
    ~Aphex_exciterAudioProcessor();

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
    AudioParameterFloat* drive;
    AudioParameterFloat* tone;
    AudioParameterFloat* mix;
    AudioParameterFloat* cost;

    DriveAmp driveAmp[2];
    Filter filter[2];
    GeneratorProcessor generatorProcessor;
    MixCircuit mixCircuit[2];
    SummingAmp summingAmp[2];
    OutputFilter outputFilter[2];

    AudioBuffer<float> dryBuffer;

    dsp::Oversampling<float> oversampling = dsp::Oversampling<float> (2, 2, dsp::Oversampling<float>::FilterType::filterHalfBandPolyphaseIIR);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Aphex_exciterAudioProcessor)
};
