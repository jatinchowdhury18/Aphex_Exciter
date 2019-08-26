#ifndef RECTIFIERPART2_H_INCLUDED
#define RECTIFIERPART2_H_INCLUDED

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
        z[1] = x * b[1] - y * a[1];

        return y;
    }

protected:
    float b[2] = { 1.0f, 0.0f };
    float a[2] = { 1.0f, 0.0f };
    float z[2] = { 1.0f, 0.0f };

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OnePole)
};

class InputFilter : public OnePole
{
public:
    InputFilter () {}
    ~InputFilter () {}

    void reset (float sampleRate)
    {
        T = 1.0f / sampleRate;
        c = 2.0f / T;

        OnePole::reset();
    }

    void calcCoefs (float R_3, float C)
    {
        // Laplace domain coefficients
        float b1 = -R_3 * C;
        float b0 = 0;
        float a1 = R_3;
        float a0 = R_3 * R_3;
        
        // bilinear transform
        a[0] = a0 * c + a1;
        a[1] = (-a0 * c + a1) / a[0];
        b[0] = (b0 * c + b1) / a[0];
        b[1] = (-b0 * c + b1) / a[0];
    }

private:
    float T = 1.0f / 44100.0f;
    float c = 2.0f / T;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InputFilter)
};

class ControlFilter : public OnePole
{
public:
    ControlFilter () {}
    ~ControlFilter () {}

    void reset (float sampleRate)
    {
        T = 1.0f / sampleRate;
        c = 2.0f / T;

        OnePole::reset();
    }

    void calcCoefs (float R_2, float R_3, float C)
    {
        // Laplace domain coefficients
        float b1 = -R_3 * C;
        float b0 = 0;
        float a1 = R_2;
        float a0 = R_2 * R_3;

        // bilinear transform
        a[0] = a0 * c + a1;
        a[1] = (-a0 * c + a1) / a[0];
        b[0] = (b0 * c + b1) / a[0];
        b[1] = (-b0 * c + b1) / a[0];
    }

private:
    float T = 1.0f / 44100.0f;
    float c = 2.0f / T;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlFilter)
};

class RectifierPart2
{
public:
    RectifierPart2() {}
    ~RectifierPart2() {}

    void prepare (float sampleRate)
    {
        inputFilter.reset (sampleRate);
        controlFilter.reset (sampleRate);

        inputFilter.calcCoefs (R_1, C);
        controlFilter.calcCoefs (R_2, R_1, C);
    }

    float process (float x, float v_c)
    {
        return -1.0f * inputFilter.processSample (x) - controlFilter.processSample (v_c);
    }

private:
    const float R_1 = 20000.0f;
    const float R_2 = 10000.0f;
    const float C = (float) 20.0e-12;

    InputFilter inputFilter;
    ControlFilter controlFilter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RectifierPart2)

};

#endif //RECTIFIERPART2_H_INCLUDED
