/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Aphex_exciterAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Aphex_exciterAudioProcessorEditor (Aphex_exciterAudioProcessor&);
    ~Aphex_exciterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Aphex_exciterAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Aphex_exciterAudioProcessorEditor)
};
