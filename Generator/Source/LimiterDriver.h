#ifndef LIMITERDRIVER_H_INCLUDED
#define LIMITERDRIVER_H_INCLUDED

#include "OnePole.h"

class LimiterDriver : public OnePole
{
public:
    LimiterDriver() {}
    ~LimiterDriver() {}

    void reset (float sampleRate)
    {
        T = 1.0f / sampleRate;
        c = 2.0f / T;

        OnePole::reset();
        calcCoefs();
    }

    void calcCoefs()
    {
        // Laplace domain coefficients
        float b0 = R_l1;
        float b1 = 0.0f;
        float a0 = 1.0f;
        float a1 = R_l * C_l;

        // bilinear transform
        a[0] = a1 * c + a0;
        a[1] = (-a1 * c + a0) / a[0];
        b[0] = (b1 * c + b0) / a[0];
        b[1] = (-b1 * c + b0) / a[0];
        a[0] /= a[0];
    }

private:
    const float R_l = 510000.0f;
    const float R_l1 = 1.0f;
    const float C_l = (float) 100.0e-9;

    float T = 1.0f / 44100.0f;
    float c = 2.0f / T;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LimiterDriver)
};

#endif //LIMITERDRIVER_H_INCLUDED
