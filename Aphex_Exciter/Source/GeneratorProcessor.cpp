#include "GeneratorProcessor.h"

void GeneratorProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampling.initProcessing (samplesPerBlock);

    for (int ch = 0; ch < 2; ++ch)
        limiterDriver[ch].reset (oversampling.getOversamplingFactor() * (float) sampleRate);

    controlBuffer.setSize (2, (int) oversampling.getOversamplingFactor() * samplesPerBlock);
}

void GeneratorProcessor::releaseResources()
{
    oversampling.reset();
}

void GeneratorProcessor::processBlock (AudioBuffer<float>& buffer)
{
    ScopedNoDenormals noDenormals;

    // upsample
    dsp::AudioBlock<float> block (buffer);
    dsp::AudioBlock<float> osBlock = oversampling.processSamplesUp (block);

    float* ptrArray[] {osBlock.getChannelPointer (0), osBlock.getChannelPointer (1)};
    AudioBuffer<float> osBuffer (ptrArray, 2, (int) osBlock.getNumSamples());

    controlBuffer.clear();
    for (int ch = 0; ch < osBuffer.getNumChannels(); ++ch)
        controlBuffer.copyFrom (ch, 0, osBuffer, ch, 0, osBuffer.getNumSamples());

    for (int ch = 0; ch < osBuffer.getNumChannels(); ++ch)
    {
        auto x = controlBuffer.getWritePointer (ch);
        rect[ch].process (x, osBuffer.getNumSamples());
        limiterDriver[ch].process (x, osBuffer.getNumSamples());
        generator[ch].process (osBuffer.getWritePointer (ch), x, osBuffer.getNumSamples());
    }

    oversampling.processSamplesDown (block);
}
