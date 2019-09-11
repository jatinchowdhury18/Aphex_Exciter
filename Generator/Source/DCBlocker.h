#ifndef DCBLOCKER_H_INCLUDED
#define DCBLOCKER_H_INCLUDED

#include "JuceHeader.h"

class DCBlocker
{
public:
    DCBlocker() {}

    void reset (double sampleRate)
    {
        for (int n = 0; n < 3; ++n)
            z[n] = 0.0f;

        fs = (float) sampleRate;
    }

    void calcCoefs (float fc, float Q)
    {
        float wc = MathConstants<float>::twoPi * fc / fs;
        float c = 1.0f / dsp::FastMathApproximations::tan (wc / 2.0f);
        float phi = c * c;
        float K = c / Q;
        float a0 = phi + K + 1.0f;

        b[0] = phi / a0;
        b[1] = -2.0f * b[0];
        b[2] = b[0];
        a[1] = 2.0f * (1.0f - phi) / a0;
        a[2] = (phi - K + 1.0f) / a0;
    }

    inline void process (float* buffer, int numSamples)
    {
        for (int n = 0; n < numSamples; ++n)
            buffer[n] = processSample (buffer[n]);
    }

    inline float processSample (float x)
    {
        // direct form II transposed
        float y = z[1] + x * b[0];

        z[1] = z[2] + x*b[1] - y*a[1];
        z[2] = x*b[2] - y*a[2];

        return y;
    }

private:
    float a[3] = { 0.0f, 0.0f, 0.0f };
    float b[3] = { 1.0f, 0.0f, 0.0f };

    float z[3] = { 0.0f, 0.0f, 0.0f };

    float fs = 44100.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DCBlocker)
};

#endif //DCBLOCKER_H_INCLUDED
