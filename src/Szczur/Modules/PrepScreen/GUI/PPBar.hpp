#pragma once 

#include "../PPContainer.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"

namespace rat
{
    class Widget; class ImageWidget;

    class PPBar
    {
    public:
        PPBar(PPContainer& source);
        void setCenter(float x, float y);
        void setCenter(const sf::Vector2f& center);

        void setParent(Widget* parent);
    protected:
        PPContainer& _source;

        void recalcOrigin();

        Widget* _background{nullptr};
    };

}