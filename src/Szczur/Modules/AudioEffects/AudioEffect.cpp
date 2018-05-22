#include "AudioEffect.hpp"

namespace rat
{
    AudioEffect::AudioEffect(unsigned int& source)
        : _effects(source, source, source)
    {

    }

    void AudioEffect::cleanAllEffects()
    {
        cleanEffect<Equalizer>();
        cleanEffect<Reverb>();
        cleanEffect<Echo>();
    }

    unsigned int AudioEffect::getLastFreeSlot() 
    {
        for (unsigned int i = 0; i < MAX_AUX_FOR_SOURCE; ++i) {
            if (!_freeSlots[i]) {
                _freeSlots[i] = true;
                return i;
            }
        }
        LOG_INFO("OpenAL Error: Too many effects loaded into the Audio Source");
        return MAX_AUX_FOR_SOURCE;
    }
}