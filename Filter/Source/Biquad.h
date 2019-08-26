#ifndef BIQUAD_H_INCLUDED
#define BIQUAD_H_INCLUDED

#include "JuceHeader.h"

class Biquad
{
public:
    Biquad() {}
    ~Biquad() {}

    void reset()
    {
        z[0] = 0.0f;
        z[1] = 0.0f;
        z[2] = 0.0f;
    }

    virtual inline void process (float* buffer, int numSamples)
    {
        for (int n = 0; n < numSamples; ++n)
            buffer[n] = processSample (buffer[n]);
    }

    virtual inline float processSample (float x)
    {
        // direct form II transposed
        float y = z[1] + x * b[0];

        z[1] = z[2] + x * b[1] - y * a[1];
        z[2] = x*b[2] - y*a[2];

        return y;
    }

protected:
    float b[3] = { 1.0f, 0.0f, 0.0f };
    float a[3] = { 1.0f, 0.0f, 0.0f };
    float z[3] = { 1.0f, 0.0f, 0.0f };

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Biquad)
};

#endif //BIQUAD_H_INCLUDED
