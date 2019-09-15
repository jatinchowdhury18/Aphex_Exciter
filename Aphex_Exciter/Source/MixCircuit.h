#ifndef MIXCIRCUIT_H_INCLUDED
#define MIXCIRCUIT_H_INCLUDED

#include "../../Generator/Source/OnePole.h"

class MixCircuit : public OnePole
{
public:
    MixCircuit() :
        R_1_rand (getTolerance (R_1)),
        C_1_rand (getTolerance (C_1)),
        R_p_rand (getTolerance (R_p))
    {
        mix.reset (100);
    }
    ~MixCircuit() {}

    void setMix (float value)
    {
        mix.setTargetValue (value);
    }

    void updateMix (float value)
    {
        float R_p_now = R_p + error * R_p_rand;
        R_p_low = powf (R_p_now, value);
        calcCoefs();
    }

    void setCost (float cost)
    {
        error = 1.0f - cost;

        updateMix (mix.getCurrentValue());
        calcCoefs();
    }

    void reset (float sampleRate)
    {
        fs = sampleRate;

        mix.setCurrentAndTargetValue (mix.getTargetValue());
        updateMix (mix.getCurrentValue());

        OnePole::reset();
        calcCoefs();
    }

    inline void calcCoefs()
    {
        float R_1_now = R_1 + error * R_1_rand;
        float R_p_now = R_p + error * R_p_rand;
        float C_1_now = C_1 + error * C_1_rand;

        float b0 = C_1_now;
        float b1 = 0.0f;
        
        float a0 = C_1_now * (R_1_now+ R_p_now);
        float a1 = 1.0f;

        float c = 2.0f * fs;

        a[0] = a0 * c + a1;
        a[1] = (-a0 * c + a1) / a[0];
        b[0] = (b0 * c + b1) / a[0];
        b[1] = (-b0 * c + b1) / a[0];
        a[0] /= a[0];
    }

    inline void process (float* buffer, int numSamples) override
    {
        for (int n = 0; n < numSamples; ++n)
        {
            if (mix.isSmoothing())
                updateMix (mix.getNextValue());

            // Process sample outputs current. For output voltage multiply by low half of pot
            buffer[n] = R_p_low * processSample (buffer[n]);
        }
    }

private:
    float getTolerance (float value)
    {
        return (Random::getSystemRandom().nextFloat() - 0.5f) * (value * 1.0f); // +/- 50%
    }

    const float R_1 = 150000.0f;
    const float C_1 = (float) 4.7e-6;
    const float R_p = 10000.0f;
    float R_p_low = 0.0f;

    const float R_1_rand;
    const float C_1_rand;
    const float R_p_rand;

    SmoothedValue<float, ValueSmoothingTypes::Linear> mix = 0.0f;
    float error = 0.0f;

    float fs = 44100.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixCircuit)
};

#endif //MIXCIRCUIT_H_INCLUDED
