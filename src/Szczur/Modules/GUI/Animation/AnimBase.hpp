#pragma once

#include "AnimData.hpp"

namespace rat
{
namespace gui
{
    enum AnimType : int
    {
        Pos = 1 << 0,
        Color = 1 << 1,
        Scroll = 1 << 2,
        Text = 1 << 3
    };
    class AnimBase
    {
    public:

        AnimBase(AnimType type) : _type{type} {}

        void update(float dt)
        {
            if(!_started)
            {
                _easing = EasingFuncs::get(_data.easing);
            }

            _currentTime += dt;

            if(_currentTime >= _data.inTime)
            {
                _finish();
                _data.onFinishCallback();
                _isAlive = false;
                _started = false;
            }
            else _update();
        }
        bool isAlive() const { return _isAlive; }

        void setHoldTime(float time) 
        {
            _data.inTime = time;
        }
        void setAnimData(const AnimData& data)
        {
            _data = data;
        }
        AnimType getType() const { return _type; }
    protected:
        float _getTimeProp() const { return _easing(_currentTime / _data.inTime); }

        virtual void _update(){}
        virtual void _finish(){}
    private:
        float _currentTime{0.f};

        AnimData _data;

        bool _started{false};
        std::function<float(float)> _easing;
        bool _isAlive{true};

        const AnimType _type;
    };
}
}