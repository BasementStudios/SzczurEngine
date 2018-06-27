namespace rat
{
    template <typename T>
    unsigned int Effect::getAuxiliaryEffect()
    {
        return get<T>().getAuxiliaryEffect();
    }

    template <typename T>
    void Effect::sendAuxiliaryEffectsTo(T& ratAudio)
    {
        sendAuxIfCreated<Equalizer, T>(ratAudio);
        sendAuxIfCreated<Echo, T>(ratAudio);
        sendAuxIfCreated<Reverb, T>(ratAudio);
    }

    template <typename Effect_t, typename RatAudio_t>
    void Effect::sendAuxIfCreated(RatAudio_t& ratAudio)
    {
        if (get<Effect_t>().created())
            ratAudio.template setEffect<Effect_t>(getAuxiliaryEffect<Effect_t>());
    }
}