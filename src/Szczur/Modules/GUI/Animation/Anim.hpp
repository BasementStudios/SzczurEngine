#pragma once

#include <functional>
#include <array>

#include <SFML/Graphics/Color.hpp>

#include "AnimBase.hpp"
#include "BetweenGetter.hpp"

namespace rat
{
namespace gui
{
    template<typename W, AnimType type, typename T>
    class Anim : public AnimBase
    {
    public:
        using Setter_t =  std::function<void (W*, const T&)>;
        Anim(W* owner, Setter_t setter)
        :
        AnimBase(type),
        _owner(owner),
        _setter(setter)
        {
        }
        void setAnim(const T& fromValue, const T& toValue, float inTime)
        {
            _start = fromValue;
            _end = toValue;
            setHoldTime(inTime);
        }
        void setAnim(const T& fromValue, const T& toValue, const AnimData& data)
        {
            _start = fromValue;
            _end = toValue;
            setAnimData(data);
        }

    protected:
        virtual void _finish() override
        {
            _setter(_owner, _end);
        }
        virtual void _update() override
        {
            float prop = _getTimeProp();
            _setter(_owner, _bGetter(_start, _end, prop));
        }

    
    private:
        W  * const _owner{nullptr};
        const Setter_t _setter;

        BetweenGetter<T> _bGetter;

        T _start;
        T _end;
    };
}
}