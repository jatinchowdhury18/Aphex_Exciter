#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

#include "Biquad.h"

class Filter : public Biquad
{
public:
    Filter() :
        R_1_rand (getTolerance (R_1)),
        R_2_rand (getTolerance (R_2)),
        R_3_rand (getTolerance (R_3)),
        C_1_rand (getTolerance (C_1)),
        C_2_rand (getTolerance (C_2)),
        C_3_rand (getTolerance (C_3))
    {
        tone.reset (50);
    }

    ~Filter() {}

    void setTone (float value)
    {
        tone.setTargetValue (value);
    }

    void updateTone (float value)
    {
        R_p = powf (50000.0f, 1.0f - value);
        calcCoefs();
    }

    void setCost (float cost)
    {
        error = 1.0f - cost;
        calcCoefs();
    }

    void reset (float sampleRate)
    {
        fs = sampleRate;

        tone.setCurrentAndTargetValue (tone.getCurrentValue());
        updateTone (tone.getCurrentValue());

        Biquad::reset();
        calcCoefs();
    }

    void calcCoefs()
    {
        float R_1_now = R_1 + error * R_1_rand;
        float R_2_now = R_2 + error * R_2_rand;
        float R_3_now = R_3 + error * R_3_rand;
        float C_1_now = C_1 + error * C_1_rand;
        float C_2_now = C_2 + error * C_2_rand;
        float C_3_now = C_3 + error * C_3_rand;

        float R_1p = R_1_now + R_p;

        float a2 = R_1p * R_1p * C_1_now * C_1_now * R_2_now * C_3_now;
        float a1 = 4.0f * R_1p * C_1_now * R_2_now * (R_3_now * C_2_now + 2.0f * C_3_now);
        float a0 = 4.0f * R_1p * C_1_now * R_3_now;

        float b2 = 0;
        float b1 = 0;
        float b0 = R_2_now * C_3_now;

        // find freq to match with bilinear transform
        float T = 1.0f / fs;
        float wc = sqrtf (abs (a1*a1 - 4.0f*a0*a2)) / (2.0f * a2);
        float c = wc / dsp::FastMathApproximations::tan (wc * T / 2.0f); // (2.0f * fs);
        float c_2 = c*c;

        // bilinear
        a[0] = a2 * c_2 + a1 * c + a0;
        a[1] = 2.0f * (a0 - a2 * c_2) / a[0];
        a[2] = (a2 * c_2 - a1 * c + a0) / a[0];

        b[0] = (b2 * c_2 + b1 * c + b0) / a[0];
        b[1] = 2.0f * (b0 - b2 * c_2) / a[0];
        b[2] = (b2 * c_2 - b1 * c + b0) / a[0];
    }

    inline void process (float* buffer, int numSamples) override
    {
        for (int n = 0; n < numSamples; ++n)
        {
            if (tone.isSmoothing())
                updateTone (tone.getNextValue());

            buffer[n] = processSample (buffer[n]);
        }
    }

    inline float processSample (float x) override
    {
        float y = z[1] + x * b[0];
        z[1] = z[2] + x * b[1] - y * a[1];
        z[2] = x * b[2] - y * a[2];

        return y;
    }

private:
    float getTolerance (float value)
    {
        return (Random::getSystemRandom().nextFloat() - 0.5f) * (value * 0.5f); // +/- 25%
    }

    const float R_1 = 10000;
    const float R_2 = 2400;
    const float R_3 = 1000;
    const float C_1 = (float) 4.7e-9;
    const float C_2 = (float) 56.0e-12;
    const float C_3 = (float) 4.7e-6;
    float R_p = 0.0f;

    const float R_1_rand;
    const float R_2_rand;
    const float R_3_rand;
    const float C_1_rand;
    const float C_2_rand;
    const float C_3_rand;

    SmoothedValue<float, ValueSmoothingTypes::Linear> tone = 0.0f;
    float error = 0.0f;

    float fs = 44100.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter)
};

#endif //FILTER_H_INCLUDED
