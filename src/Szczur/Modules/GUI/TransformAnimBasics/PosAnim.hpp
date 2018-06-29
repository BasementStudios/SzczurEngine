#pragma once

#include "TransformAnimationBase.hpp"


#include <SFML/Graphics.hpp>

namespace rat
{
    class PositionAnim : public TransformAnimationBase
    {
    public:
        PositionAnim();
        virtual void update(float dt) override;
        void setAnim(const sf::Vector2f& fromValue, const sf::Vector2f& toValue, float inTime);
        sf::Vector2f getActualPos() const;
    private:
        sf::Vector2f _start;
        sf::Vector2f _end;
    };
}