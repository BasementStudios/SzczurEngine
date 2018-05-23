namespace rat
{
    template <typename T>
    T& AudioEffect::getEffect()
    {
        auto& effect = std::get<T>(_effects);

        if (!effect.created())
            effect.init(getLastFreeSlot());

        return effect;
    }

    template <typename T>
    T& AudioEffect::get()
    {
        return std::get<T>(_effects);
    }

    template <typename T>
    void AudioEffect::setEffect(unsigned int aux)
    {
        getEffect<T>().setAuxiliaryEffect(aux);
    }

    template <typename T>
    void AudioEffect::cleanEffect()
    {
        auto& effect = std::get<T>(_effects);
        _freeSlots[effect.getSlot()] = false;
        effect.cleanEffect();
    }

}