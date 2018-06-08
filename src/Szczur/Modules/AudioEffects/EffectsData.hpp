#pragma once

namespace rat
{
    struct EqualizerData 
    {
        float lowCutoff             = 200;
        float lowGain               = 1;
        float highCutoff            = 6000;
        float highGain              = 1;
        float lowMidCenter          = 500;
        float lowMidWidth           = 1;
        float lowMidGain            = 1;
        float highMidCenter         = 3000;
        float highMidWidth          = 1;
        float highMidGain           = 1;
    };

    struct ReverbData
    {
        float density               = 1;
        float diffusion             = 1;
        float gain                  = 0.32;
        float gainHf                = 0.89;
        float decayTime             = 1.49;
        float decayHfRatio          = 0.83;
        float reflectionsGain       = 0.05;
        float reflectionsDelay      = 0.007;
        float lateReverbGain        = 1.26;
        float lateReverbDelay       = 0.011;
        float airAbsorptionGainHf   = 0.994;
        float roomRolloffFactor     = 0;
        bool decayHfLimit           = true;
    };

    struct EchoData
    {
        float delay                 = 0.1;
        float lrDelay               = 0.1;
        float damping               = 0.5;
        float feedback              = 0.5;
        float spread                = -1;
    };
}