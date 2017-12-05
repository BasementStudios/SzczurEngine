#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include "Widget.hpp"

namespace rat {
    class ImageWidget : public Widget {
    public:
        ImageWidget();
        ImageWidget(const std::string& path);

        void setTexture(const std::string& path);
    
    protected:
        virtual sf::Vector2u _getSize() const override;
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        sf::Sprite _sprite;
        sf::Texture _texture;

        
    };
}