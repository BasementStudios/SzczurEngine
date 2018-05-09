#pragma once

#include "BaseBar.hpp"

#include <string>
#include <array>

#include <SFML/Graphics/Texture.hpp>

namespace rat
{
    class GlyphContainer;
    class GUI;
    class GlyphBar : public BaseBar
    {
    public:
        GlyphBar();
        GlyphBar(const std::string& type);

        void setStartAmount(int amount);
        int getUsedAmount() const;
        void initAssetsViaGUI(GUI& gui);
        void setType(const std::string& type);
        
    private:    
        std::string _type;
        int _amount{0};
        int _takenAmount{0};

        std::array<sf::Texture*, 4> _glyphTextures;


        constexpr static float _circleShiftProp = 0.14644660940672f;

        ImageWidget* _container{nullptr};
        ImageWidget* _glyph{nullptr};
        TextWidget* _amountState{nullptr};

        void _onClick();
        void _updateText();
    };
}