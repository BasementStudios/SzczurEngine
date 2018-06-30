#include "TransformAnimationBase.hpp"

#include <algorithm>

namespace rat
{
        TransformAnimationBase::Types TransformAnimationBase::getType() const
        {
            return _type;
        }
        bool TransformAnimationBase::isAnimationOver() const
        {
            return _currentTime >= _holdTime;
        }

        void TransformAnimationBase::_advanceTime(float dt)
        {
            _currentTime += dt;
            _currentTime = std::min(_holdTime, _currentTime);
        }
        float TransformAnimationBase::_getCurrentTime() const
        {
            return _currentTime;
        }
        void TransformAnimationBase::_setHoldTime(float holdTime)
        {
            _holdTime = holdTime;
        }
        void TransformAnimationBase::_setType(Types type)
        {
            _type = type;
        }
        float TransformAnimationBase::_getTimeProp() const
        {
            if(_holdTime == 0.f) return 0.f;
            return _currentTime / _holdTime;
        }
}