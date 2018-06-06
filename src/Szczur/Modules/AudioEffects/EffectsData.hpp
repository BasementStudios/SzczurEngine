#pragma once

namespace rat
{
    struct EqualizerData 
    {
        float lowCutoff;
        float lowGain;
        float highCutoff;
        float highGain;
        float lowMidCenter;
        float lowMidWidth;
        float lowMidGain;
        float highMidCenter;
        float highMidWidth;
        float highMidGain;
    };

    struct ReverbData
    {
        float density;
        float diffusion;
        float gain;
        float gainHf;
        float decayTime;
        float decayHfRatio;
        float reflectionsGain;
        float reflectionsDelay;
        float lateReverbGain;
        float lateReverbDelay;
        float airAbsorptionGainHf;
        float roomRolloffFactor;
        bool decayHfLimit;
    };

    struct EchoData
    {
        float delay;
        float lrDelay;
        float damping;
        float feedback;
        float spread;
    };
}