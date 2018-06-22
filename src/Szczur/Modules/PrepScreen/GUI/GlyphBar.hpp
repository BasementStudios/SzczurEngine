#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include <string>
#include <array>

#include <SFML/Graphics/Texture.hpp>

#include "../GlyphTypes.hpp"  

namespace rat
{
    class GlyphContainer;
    class GrayPPArea;
    class GlyphBar : public BaseBar
    {
    public:
        GlyphBar();

        void setType(GlyphID glyph);
        void setStartAmount(int amount);
        int getUsedAmount() const;
        void initAssetsViaGUI(GUI& gui);

        void setAmount(size_t activated, size_t total);
        
    private:    
        GlyphID _type;
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