#include "GeneratorProcessor.h"

void GeneratorProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (int ch = 0; ch < 2; ++ch)
        limiterDriver[ch].reset ((float) sampleRate);

    controlBuffer.setSize (2, samplesPerBlock);
}

void GeneratorProcessor::releaseResources()
{
}

void GeneratorProcessor::processBlock (AudioBuffer<float>& buffer)
{
    ScopedNoDenormals noDenormals;
    auto numSamples = buffer.getNumSamples();

    controlBuffer.clear();
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        controlBuffer.copyFrom (ch, 0, buffer, ch, 0, buffer.getNumSamples());

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto x = controlBuffer.getWritePointer (ch);
        rect[ch].process (x, buffer.getNumSamples());
        limiterDriver[ch].process (x, buffer.getNumSamples());
        generator[ch].process (buffer.getWritePointer (ch), x, buffer.getNumSamples());
    }
}
