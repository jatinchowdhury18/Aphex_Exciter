#ifndef RECTIFIER_H_INCLUDED
#define RECTIFIER_H_INCLUDED

#include "JuceHeader.h"
/*
class Differentiator
{
public:
    Differentiator() {}
    ~Differentiator() {}

    void reset()
    {
        for (int n = 0; n < order; ++n)
            z[n] = 0.0f;
    }

    inline float process (float x)
    {
        return ((z[4] - x) * coef1 + (z[1] - z[3]) * coef2) / norm;
    }

    inline void update (float x)
    {
        z[4] = z[3];
        z[3] = z[2];
        z[2] = z[1];
        z[1] = z[0];
        z[0] = x;
    }

private:
    static const int order = 5;
    float z[order] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

    const float coef1 = 3.0f / 16.0f;
    const float coef2 = 31.0f / 32.0f;
    const float norm = 1.2f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Differentiator)
};
*/
class Rectifier
{
public:
    Rectifier() {}
    ~Rectifier() {}

    inline void process (float* buffer, int numSamples)
    {
        for (int n = 0; n < numSamples; ++n)
            buffer[n] = diode (0.05f * buffer[n]);
    }

private:
    // diode values
    const float v_t = 25.9f / 1000.0f;
    const float I_s = 0.2f;

    inline float diode (float x)
    {
        return I_s * (expf (x / v_t) - 1.0f);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Rectifier)
};

#endif //RECTIFIER_H_INCLUDED
