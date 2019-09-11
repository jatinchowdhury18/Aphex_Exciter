#ifndef GENERATOR_H_INCLUDED
#define GENERATOR_H_INCLUDED

#include "OnePole.h"

class Generator
{
public:
    Generator() {}
    ~Generator() {}

    inline void process (float* buffer, float* controlBuffer, int numSamples)
    {
        for (int n = 0; n < numSamples; ++n)
            buffer[n] = processSample (buffer[n], controlBuffer[n]);
    }

    inline float processSample (float input, float control)
    {
        return TAmp (input, 100.0f*control);
    }

private:
    // Transconducting Amp
    inline float TAmp (float V_in, float I_abc)
    {
        return I_abc * tanhf (V_in / v_tOverTwo);
    }
    const float v_tOverTwo = 25.9f / 1000.0f / 2.0f;

    inline float myTanh (float x) noexcept
    {
        if (abs (x) > 5.0f)
            return (x > 0.0f) ? 1.0f : -1.0f;

        auto x2 = x * x;
        auto numerator = x * (135135 + x2 * (17325 + x2 * (378 + x2)));
        auto denominator = 135135 + x2 * (62370 + x2 * (3150 + 28 * x2));
        return numerator / denominator;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Generator)
};

#endif //GENERATOR_H_INCLUDED
