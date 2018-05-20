#pragma once

#include "Widget2.hpp"

namespace rat
{
    class TextWidget2 : public Widget2
    {
    public:
    protected:
        virtual void _recalculateComponentsPosition() {}
        virtual sf::Vector2f _getComponentsSize() const { return {}; }
        virtual void _recalculateComponentsSize() {}
        virtual void _drawComponents(sf::RenderTarget& target, sf::RenderStates) const {}
    private:
    };
}