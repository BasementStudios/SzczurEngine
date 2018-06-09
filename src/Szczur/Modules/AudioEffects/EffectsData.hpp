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

        EqualizerData() {
            reset();
        }

        void reset() {
            lowCutoff             = 200;
            lowGain               = 1;
            highCutoff            = 6000;
            highGain              = 1;
            lowMidCenter          = 500;
            lowMidWidth           = 1;
            lowMidGain            = 1;
            highMidCenter         = 3000;
            highMidWidth          = 1;
            highMidGain           = 1;
        }
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

        ReverbData() {
            reset();
        }

        void reset() {
            density               = 1;
            diffusion             = 1;
            gain                  = 0.32;
            gainHf                = 0.89;
            decayTime             = 1.49;
            decayHfRatio          = 0.83;
            reflectionsGain       = 0.05;
            reflectionsDelay      = 0.007;
            lateReverbGain        = 1.26;
            lateReverbDelay       = 0.011;
            airAbsorptionGainHf   = 0.994;
            roomRolloffFactor     = 0;
            decayHfLimit           = true;
        }
    };

    struct EchoData
    {
        float delay;
        float lrDelay;
        float damping;
        float feedback;
        float spread;

        EchoData() {
            reset();
        }

        void reset() {
            delay                 = 0.1;
            lrDelay               = 0.1;
            damping               = 0.5;
            feedback              = 0.5;
            spread                = -1;
        }
    };
}