#pragma once

#include "PPBar.hpp"

/*
#include "../PPContainer.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
*/

namespace rat
{
    class ImageWidget; class GUI;

    class ColoredPPBar : public PPBar
    {
    public:
        ColoredPPBar(PPContainer& source);
        void recalculate();
       // void setCenter(float x, float y);
        //void setCenter(const sf::Vector2f& center);

        void initTexturesViaGui(GUI& gui);
        //void setParent(Widget* parent);
       

    private:
        std::map<std::string, ImageWidget*> _pps;
        std::map<std::string, sf::Texture*> _textures;
    };
}