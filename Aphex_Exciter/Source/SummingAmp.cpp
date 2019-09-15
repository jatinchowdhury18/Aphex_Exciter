#include "SummingAmp.h"

SummingAmp::SummingAmp() :
    R_1_rand (getTolerance (R_1)),
    R_2_rand (getTolerance (R_2)),
    C_1_rand (getTolerance (C_1)),
    C_2_rand (getTolerance (C_2))
{
}

SummingAmp::~SummingAmp()
{
}

void SummingAmp::setCost (float cost)
{
    error = 1.0f - cost;
    calcCoefs();
}

void SummingAmp::calcCoefs()
{
    float R_1_now = R_1 + error * R_1_rand;
    float R_2_now = R_2 + error * R_2_rand;
    float C_1_now = C_1 + error * C_1_rand;
    float C_2_now = C_2 + error * C_2_rand;

    float RC_1 = R_1_now * C_1_now;
    float RC_2 = R_2_now * C_2_now;

    float a2 = RC_1 * RC_2;
    float a1 = RC_1 + RC_2;
    float a0 = 1.0f;

    float b2 = 0.0f;
    float b1 = R_2_now * C_1_now;
    float b0 = 0.0f;

    // find freq to match with bilinear transform
    float c = 2.0f * fs;
    float c_2 = c*c;

    // bilinear
    a[0] = a2 * c_2 + a1 * c + a0;
    a[1] = 2.0f * (a0 - a2 * c_2) / a[0];
    a[2] = (a2 * c_2 - a1 * c + a0) / a[0];

    b[0] = (b2 * c_2 + b1 * c + b0) / a[0];
    b[1] = 2.0f * (b0 - b2 * c_2) / a[0];
    b[2] = (b2 * c_2 - b1 * c + b0) / a[0];
}

void SummingAmp::reset (float sampleRate)
{
    fs = sampleRate;

    for (int n = 0; n < 3; ++n)
    {
        zDry[n] = 0.0f;
        zWet[n] = 0.0f;
    }

    calcCoefs();
}

float SummingAmp::getTolerance (float value)
{
    return (Random::getSystemRandom().nextFloat() - 0.5f) * (value * 1.0f); // +/- 50%
}
