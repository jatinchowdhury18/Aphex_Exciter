#ifndef DRIVEAMP_H_INCLUDED
#define DRIVEAMP_H_INCLUDED

#include "Biquad.h"

class DriveAmp : public Biquad
{
public:
    DriveAmp() :
        R_1_rand (getTolerance (R_1)),
        C_1_rand (getTolerance (C_1)),
        C_2_rand (getTolerance (C_2))
    { 
        drive.reset (100); 
    }
    ~DriveAmp() {}

    void setDrive (float value)
    {
        drive.setTargetValue (value);
    }

    void updateDrive (float value)
    {
        R_p = 150.0f + powf (50000.0f, 1.0f - value);
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

        drive.setCurrentAndTargetValue (drive.getTargetValue());
        updateDrive (drive.getCurrentValue());

        Biquad::reset();
        calcCoefs();
    }

    void calcCoefs()
    {
        float T = 1.0f / fs;
        float R_1_now = R_1 + error * R_1_rand;
        float C_1_now = C_1 + error * C_1_rand;
        float C_2_now = C_2 + error * C_2_rand;

        float a2 = R_1_now * R_p * C_1_now * C_2_now;
        float a1 = R_1_now * C_2_now + R_p * C_1_now;
        float a0 = 1.0f;

        float b2 = a2;
        float b1 = a1 + R_1_now * C_1_now;
        float b0 = a0;

        // bilinear
        a[0] = 4.0f * a2 + 2.0f * T * a1 + T * T * a0;
        a[1] = (2.0f * T * T * a0 - 8.0f * a2) / a[0];
        a[2] = (4.0f * a2 - 2.0f * T * a1 + T * T * a0) / a[0];

        b[0] = (4.0f * b2 + 2.0f * T * b1 + T * T * b0) / a[0];
        b[1] = (2.0f * T * T * b0 - 8.0f * b2) / a[0];
        b[2] = (4.0f * b2 - 2.0f * T * b1 + T * T * b0) / a[0];
    }

    inline float clip (float x)
    {
        x /= 18.0f;
        if (x <= -1.0f)
            x = -18.0f;
        else if (x >= 1.0f)
            x = 18.0f;
        else
        {
            x -= x*x*x / 3.0f;
            x *= 27.0f; // 18 * (3/2)
        }
        return x;
    }

    inline void process (float* buffer, int numSamples) override
    {
        for (int n = 0; n < numSamples; ++n)
        {
            if (drive.isSmoothing())
                updateDrive (drive.getNextValue());

            buffer[n] = processSample (buffer[n]);
        }
    }

    inline float processSample (float x) override
    {
        float y = z[1] + x * b[0];
        z[1] = z[2] + x * b[1] - y * a[1];
        z[2] = x * b[2] - y * a[2];

        return clip (y);
    }

private:
    float getTolerance (float value)
    {
        return (Random::getSystemRandom().nextFloat() - 0.5f) * (value * 1.0f); // +/- 50%
    }

    const float R_1 = 20000.0f;
    const float C_1 = (float) 4.7e-6;
    const float C_2 = (float) 20.0e-12;
    float R_p = 150.0f;

    const float R_1_rand;
    const float C_1_rand;
    const float C_2_rand;

    SmoothedValue<float, ValueSmoothingTypes::Linear> drive = 0.0f;
    float error = 0.0f;

    float fs = 44100.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DriveAmp)
};

#endif //DRIVEAMP_H_INCLUDED
