#pragma once

namespace rat

{
    class TransformAnimationBase
    {
    public:
        enum Types 
        {
            None = 0,
            Pos = 1 << 0,
            Size = 1 << 1,
            Color = 1 << 2
        };
        virtual void update(float dt) = 0;
        Types getType() const;
        bool isAnimationOver() const;

        virtual ~TransformAnimationBase() = default;

    protected:
        void _advanceTime(float dt);
        float _getTimeProp() const;
        float _getCurrentTime() const;
        void _setHoldTime(float holdTime);
        void _setType(Types type);
    private:
        Types _type;
        float _holdTime {0.f};
        float _currentTime{0.f};
    };

}