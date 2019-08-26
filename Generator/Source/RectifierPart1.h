#ifndef RECTIFIERPART1_H_INCLUDED
#define RECTIFIERPART1_H_INCLUDED

#include "JuceHeader.h"

class RectifierPart1
{
public:
    RectifierPart1() {}
    ~RectifierPart1() {}

    void prepare (float sampleRate)
    {
        twoOverT = 2.0f * sampleRate;
        v_i = 0.0f;
        v_i_dot = 0.0f;
        v_b = 0.0f;
        v_b_dot = 0.0f;
        v_c= 0.0f;
        v_c_dot = 1.0f;
    }

    float process (float x)
    {
        // calc v_b
        v_b_dot_1 = v_b_dot;
        v_b_dot = ((v_c * ((1.0f/R_1) + (1.0f/R_2)) - diode (v_c - v_b)) / C) + v_c_dot;
        v_b_1 = v_b;
        v_b = integral (v_b_1, v_b_dot, v_b_dot_1);
        
        // calc v_i_dot
        v_i_1 = v_i;
        v_i = x;
        v_i_dot_1 = v_i_dot;
        v_i_dot = deriv (v_i, v_i_1, v_i_dot_1);
        
        // calc v_c
        v_c_1 = v_c;
        v_c = R_2 * (diode (v_b) - (v_i/R_1));
        v_c_dot_1 = v_c_dot;
        v_c_dot = deriv (v_c, v_c_1, v_c_dot_1);

        return v_c;
    }

private:
    // diode stuff
    const float v_t = 0.0259f;
    const float I_s = 0.2f;

    inline float diode (float x)
    {
        return I_s * (dsp::FastMathApproximations::exp (x / v_t) - 1.0f);
    }

    inline float diode_dot (float x)
    {
        return (I_s / v_t) * dsp::FastMathApproximations::exp (x / v_t);
    }

    //circuit stuff
    const float R_1 = 20000.0f;
    const float R_2 = 10000.0f;
    const float C = (float) 20.0e-12;

    // State space stuff
    float twoOverT = 2.0f / 44100.0f;
    float v_i, v_i_dot, v_b, v_b_dot, v_c, v_c_dot; //state variables
    float v_i_1, v_i_dot_1, v_b_1, v_b_dot_1, v_c_1, v_c_dot_1; // temp variables

    inline float deriv (float x, float x_1, float x_1_dot)
    {
        return (x - x_1) * twoOverT - x_1_dot;
    }

    inline float integral (float x_1, float x_dot, float x_1_dot)
    {
        return x_1 + (x_dot + x_1_dot) / twoOverT;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RectifierPart1)
};

#endif //RECTIFIERPART1_H_INCLUDED
