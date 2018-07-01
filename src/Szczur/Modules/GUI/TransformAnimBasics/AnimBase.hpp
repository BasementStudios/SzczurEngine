#pragma once

namespace rat
{
namespace gui
{
    class AnimBase
    {
    public:
        enum Type
        {
            Pos = 1 << 0,
            Color = 1 << 1,
            Scroll = 1 << 2
        };
        AnimBase(Type type) : _type(type) {}
        void update(float dt)
        {
            _currentTime += dt;
            if(_currentTime >= _maxTime)
            {
                _finish();
                _isAlive = false;
            }
            else _update();
        }
        bool isAlive() const { return _isAlive; }

        void setHoldTime(float time) { _maxTime = time; }
        Type getType() const { return _type; }
    protected:
        float _getTimeProp() const { return _currentTime / _maxTime; }

        virtual void _update(){}
        virtual void _finish(){}
    private:
        float _currentTime{0.f};
        float _maxTime{0.f};

        bool _isAlive{true};
        const Type _type;
    };
}
}