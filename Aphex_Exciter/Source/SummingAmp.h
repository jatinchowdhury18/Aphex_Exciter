#ifndef SUMMINGAMP_H_INCLUDED
#define SUMMINGAMP_H_INCLUDED

#include "JuceHeader.h"

class SummingAmp
{
public:
    SummingAmp();
    ~SummingAmp();

    void setCost (float cost);
    void calcCoefs();

    void reset (float sampleRate);

    inline void process (float* wetBuffer, float* dryBuffer, int numSamples)
    {
        for (int n = 0; n < numSamples; ++n)
            wetBuffer[n] = dryBuffer[n] + processSample (dryBuffer[n], zDry) - processSample (wetBuffer[n], zWet);
    }

    inline float processSample (float x, float* z)
    {
        float y = z[1] + x * b[0];
        z[1] = z[2] + x * b[1] - y * a[1];
        z[2] = x * b[2] - y * a[2];

        return y;
    }

private:
    float getTolerance (float value);

    const float R_1 = 150000.0f;
    const float R_2 = 22000.0f;
    const float C_1 = (float) 4.7e-6;
    const float C_2 = (float) 20e-12;

    const float R_1_rand;
    const float R_2_rand;
    const float C_1_rand;
    const float C_2_rand;

    float fs = 44100.0f;
    float error = 0.0f;

    float a[3] = { 1.0f, 0.0f, 0.0f };
    float b[3] = { 1.0f, 0.0f, 0.0f };
    float zWet[3] = { 1.0f, 0.0f, 0.0f };
    float zDry[3] = { 1.0f, 0.0f, 0.0f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SummingAmp)
};

#endif //SUMMINGAMP_H_INCLUDED
