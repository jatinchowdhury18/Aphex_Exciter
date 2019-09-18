/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Aphex_exciterAudioProcessor::Aphex_exciterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter (drive = new AudioParameterFloat ("drive", "Drive", 0.0f, 1.0f, 0.75f));
    addParameter (tone = new AudioParameterFloat ("tone", "Tone", 0.0f, 1.0f, 0.5f));
    addParameter (mix = new AudioParameterFloat ("mix", "Mix", 0.0f, 1.0f, 0.75f));
    addParameter (cost = new AudioParameterFloat ("cost", "Cost", 0.0f, 1.0f, 1.0f));
}

Aphex_exciterAudioProcessor::~Aphex_exciterAudioProcessor()
{
}

//==============================================================================
const String Aphex_exciterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Aphex_exciterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Aphex_exciterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Aphex_exciterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Aphex_exciterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Aphex_exciterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Aphex_exciterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Aphex_exciterAudioProcessor::setCurrentProgram (int index)
{
}

const String Aphex_exciterAudioProcessor::getProgramName (int index)
{
    return {};
}

void Aphex_exciterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Aphex_exciterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    dryBuffer.setSize (2, samplesPerBlock);

    // Set up drive amp
    driveAmp[0].setDrive (*drive);
    driveAmp[1].setDrive (*drive);

    driveAmp[0].reset (oversampling.getOversamplingFactor() * (float) sampleRate);
    driveAmp[1].reset (oversampling.getOversamplingFactor() * (float) sampleRate);

    // Set up filter
    filter[0].setTone (*tone);
    filter[1].setTone (*tone);

    filter[0].reset (oversampling.getOversamplingFactor() * (float) sampleRate);
    filter[1].reset (oversampling.getOversamplingFactor() * (float) sampleRate);

    // Set up generator
    generatorProcessor.prepareToPlay (oversampling.getOversamplingFactor() * (float) sampleRate,
                                      (int) oversampling.getOversamplingFactor() * samplesPerBlock);

    // Set up mix circuit
    mixCircuit[0].setMix (*mix);
    mixCircuit[1].setMix (*mix);

    mixCircuit[0].reset ((float) sampleRate);
    mixCircuit[1].reset ((float) sampleRate);

    // Set up summing amp
    summingAmp[0].reset ((float) sampleRate);
    summingAmp[1].reset ((float) sampleRate);

    // Set up output filter
    outputFilter[0].reset ((float) sampleRate);
    outputFilter[1].reset ((float) sampleRate);

    oversampling.initProcessing (samplesPerBlock);
}

void Aphex_exciterAudioProcessor::releaseResources()
{
    generatorProcessor.releaseResources();

    oversampling.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Aphex_exciterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void Aphex_exciterAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    buffer.applyGain (1.58f); //Input voltage

    // keep dry signal for later
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        dryBuffer.copyFrom (ch, 0, buffer, ch, 0, buffer.getNumSamples());

    buffer.applyGain (0.3125f); // Input voltage divider

    // upsample
    dsp::AudioBlock<float> block (buffer);
    dsp::AudioBlock<float> osBlock = oversampling.processSamplesUp (block);

    float* ptrArray[] {osBlock.getChannelPointer (0), osBlock.getChannelPointer (1)};
    AudioBuffer<float> osBuffer (ptrArray, 2, (int) osBlock.getNumSamples());

    for (int ch = 0; ch < osBuffer.getNumChannels(); ++ch)
    {
        // Drive Amp
        driveAmp[ch].setDrive (*drive);
        driveAmp[ch].setCost (*cost);
        driveAmp[ch].process (osBuffer.getWritePointer (ch), osBuffer.getNumSamples());

        // Filter
        filter[ch].setTone (*tone);
        filter[ch].setCost (*cost);
        filter[ch].process (osBuffer.getWritePointer (ch), osBuffer.getNumSamples());
    }

    osBuffer.applyGain ((1.0f / 54.0f) * Decibels::decibelsToGain (-12.0f));
    generatorProcessor.processBlock (osBuffer);

    // Downsample
    oversampling.processSamplesDown (block);

    buffer.applyGain (1020.0f); // Gain coming out from the generator

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        // Mix circuit
        mixCircuit[ch].setMix (*mix);
        mixCircuit[ch].setCost (*cost);
        mixCircuit[ch].process (buffer.getWritePointer (ch), buffer.getNumSamples());

        // Summing Amp
        summingAmp[ch].setCost (*cost);
        summingAmp[ch].process (buffer.getWritePointer (ch), dryBuffer.getWritePointer (ch), buffer.getNumSamples());

        // Output Filter
        outputFilter[ch].setCost (*cost);
        outputFilter[ch].process (buffer.getWritePointer (ch), buffer.getNumSamples());
    }

    buffer.applyGain (0.5f); // normalize output
}

//==============================================================================
bool Aphex_exciterAudioProcessor::hasEditor() const
{
    return false; // true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Aphex_exciterAudioProcessor::createEditor()
{
    return nullptr; // new Aphex_exciterAudioProcessorEditor (*this);
}

//==============================================================================
void Aphex_exciterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Aphex_exciterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Aphex_exciterAudioProcessor();
}
