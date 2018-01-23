#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "Widget.hpp"

namespace rat {
    class ImageWidget : public Widget {
    public:
        ImageWidget();
        ImageWidget(sf::Texture* texture);

        void setTexture(sf::Texture* texture);
    
    protected:
        virtual sf::Vector2u _getSize() const override;
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        sf::Sprite _sprite; 
    };
}