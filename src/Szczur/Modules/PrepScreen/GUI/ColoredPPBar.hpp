#pragma once
#include "../PPContainer.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"

namespace rat
{
    class ImageWidget; class GUI;

    class ColoredPPBar
    {
    public:
        ColoredPPBar(PPContainer& source, Widget* parent);
        void recalculate();
        void setCenter(float x, float y);
        void setCenter(const sf::Vector2f& center);

        void initTexturesViaGui(GUI& gui);
       

    private:
        PPContainer& _source;

        void recalcOrigin();

        Widget* _background{nullptr};
        std::map<std::string, ImageWidget*> _pps;
        std::map<std::string, sf::Texture*> _textures;
    };
}