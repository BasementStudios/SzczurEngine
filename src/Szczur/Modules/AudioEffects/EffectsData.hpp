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
            lowCutoff             = 200.f;
            lowGain               = 1.f;
            highCutoff            = 6000.f;
            highGain              = 1.f;
            lowMidCenter          = 500.f;
            lowMidWidth           = 1.f;
            lowMidGain            = 1.f;
            highMidCenter         = 3000.f;
            highMidWidth          = 1.f;
            highMidGain           = 1.f;
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
            density               = 1.f;
            diffusion             = 1.f;
            gain                  = 0.32f;
            gainHf                = 0.89f;
            decayTime             = 1.49f;
            decayHfRatio          = 0.83f;
            reflectionsGain       = 0.05f;
            reflectionsDelay      = 0.007f;
            lateReverbGain        = 1.26f;
            lateReverbDelay       = 0.011f;
            airAbsorptionGainHf   = 0.994f;
            roomRolloffFactor     = 0.f;
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
