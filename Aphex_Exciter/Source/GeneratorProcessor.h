#ifndef GENERATORPROCESSOR_H_INCLUDED
#define GENERATORPROCESSOR_H_INCLUDED

#include "../../Generator/Source/Generator.h"
#include "../../Generator/Source/Rectifier.h"
#include "../../Generator/Source/LimiterDriver.h"

class GeneratorProcessor
{
public:
    GeneratorProcessor() {}
    ~GeneratorProcessor() {}

    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock (AudioBuffer<float>&);

private:
    AudioBuffer<float> controlBuffer;

    Rectifier rect[2];
    LimiterDriver limiterDriver[2];
    Generator generator[2];

    dsp::Oversampling<float> oversampling = dsp::Oversampling<float> (2, 1, dsp::Oversampling<float>::FilterType::filterHalfBandPolyphaseIIR);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GeneratorProcessor)
};

#endif //GENERATORPROCESSOR_H_INCLUDED