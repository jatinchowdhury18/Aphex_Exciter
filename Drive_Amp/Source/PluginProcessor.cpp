/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Drive_ampAudioProcessor::Drive_ampAudioProcessor()
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
    addParameter (drive = new AudioParameterFloat ("drive", "Drive", 0.0f, 1.0f, 0.0f));
    addParameter (cost = new AudioParameterFloat ("cost", "Cost", 0.0f, 1.0f, 1.0f));
}

Drive_ampAudioProcessor::~Drive_ampAudioProcessor()
{
}

//==============================================================================
const String Drive_ampAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Drive_ampAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Drive_ampAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Drive_ampAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Drive_ampAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Drive_ampAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Drive_ampAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Drive_ampAudioProcessor::setCurrentProgram (int index)
{
}

const String Drive_ampAudioProcessor::getProgramName (int index)
{
    return {};
}

void Drive_ampAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Drive_ampAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    driveAmp[0].setDrive (*drive);
    driveAmp[1].setDrive (*drive);

    driveAmp[0].reset (oversampling.getOversamplingFactor() * (float) sampleRate);
    driveAmp[1].reset (oversampling.getOversamplingFactor() * (float) sampleRate);

    oversampling.initProcessing (samplesPerBlock);
}

void Drive_ampAudioProcessor::releaseResources()
{
    oversampling.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Drive_ampAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Drive_ampAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    buffer.applyGain (1.58f * 0.3125f);

    dsp::AudioBlock<float> block (buffer);
    dsp::AudioBlock<float> osBlock (buffer);

    osBlock = oversampling.processSamplesUp (block);

    float* ptrArray[] = {osBlock.getChannelPointer (0), osBlock.getChannelPointer (1)};
    AudioBuffer<float> osBuffer (ptrArray, 2, static_cast<int> (osBlock.getNumSamples()));

    for (int ch = 0; ch < osBuffer.getNumChannels(); ++ch)
    {
        driveAmp[ch].setDrive (*drive);
        driveAmp[ch].setCost (*cost);
        driveAmp[ch].process (osBuffer.getWritePointer (ch), osBuffer.getNumSamples());
    }

    oversampling.processSamplesDown (block);

    buffer.applyGain (1.0f / 54.0f);
}

//==============================================================================
bool Drive_ampAudioProcessor::hasEditor() const
{
    return false; //true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Drive_ampAudioProcessor::createEditor()
{
    return nullptr; //new Drive_ampAudioProcessorEditor (*this);
}

//==============================================================================
void Drive_ampAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Drive_ampAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Drive_ampAudioProcessor();
}
