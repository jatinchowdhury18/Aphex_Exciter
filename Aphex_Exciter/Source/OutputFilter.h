#ifndef OUTPUTFILTER_H_INCLUDED
#define OUTPUTFILTER_H_INCLUDED

#include "../../Generator/Source/OnePole.h"

class OutputFilter : public OnePole
{
public:
    OutputFilter() :
        R_1_rand (getTolerance (R_1)),
        C_1_rand (getTolerance (C_1)),
        R_2_rand (getTolerance (R_2))
    {}
    ~OutputFilter() {}

    void setCost (float cost)
    {
        error = 1.0f - cost;
        calcCoefs();
    }

    void reset (float sampleRate)
    {
        fs = sampleRate;

        OnePole::reset();
        calcCoefs();
    }

    inline void calcCoefs()
    {
        float R_1_now = R_1 + error * R_1_rand;
        float R_2_now = R_2 + error * R_2_rand;
        float C_1_now = C_1 + error * C_1_rand;

        float b0 = R_2_now * C_1_now;
        float b1 = 0.0f;

        float a0 = C_1_now * (R_1_now+ R_2_now);
        float a1 = 1.0f;

        float c = 2.0f * fs;

        a[0] = a0 * c + a1;
        a[1] = (-a0 * c + a1) / a[0];
        b[0] = (b0 * c + b1) / a[0];
        b[1] = (-b0 * c + b1) / a[0];
        a[0] /= a[0];
    }

private:
    float getTolerance (float value)
    {
        return (Random::getSystemRandom().nextFloat() - 0.5f) * (value * 1.0f); // +/- 50%
    }

    const float R_1 = 150.0f;
    const float C_1 = (float) 47e-6;
    const float R_2 = 150000.0f;

    const float R_1_rand;
    const float C_1_rand;
    const float R_2_rand;

    float error = 0.0f;

    float fs = 44100.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutputFilter)
};

#endif //OUTPUTFILTER_H_INCLUDED
