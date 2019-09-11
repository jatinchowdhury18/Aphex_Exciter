#ifndef ONEPOLE_H_INCLUDED
#define ONEPOLE_H_INCLUDED

#include "JuceHeader.h"

class OnePole
{
public:
    OnePole() {}
    ~OnePole() {}

    void reset()
    {
        z[0] = 0.0f;
        z[1] = 0.0f;
        y = 0.0f;
    }

    virtual inline void process (float* buffer, int numSamples)
    {
        for (int n = 0; n < numSamples; ++n)
            buffer[n] = processSample (buffer[n]);
    }

    virtual inline float processSample (float x)
    {
        // direct form II transposed
        y = z[1] + x * b[0];
        z[1] = x * b[1] - y * a[1];

        return y;
    }

protected:
    float b[2] = { 1.0f, 0.0f };
    float a[2] = { 1.0f, 0.0f };

private:
    float z[2] = { 1.0f, 0.0f };
    float y = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OnePole)
};

#endif //ONEPOLE_H_INCLUDED
