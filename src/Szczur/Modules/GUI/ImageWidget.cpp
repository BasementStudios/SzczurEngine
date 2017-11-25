#include "ImageWidget.hpp"

#include <iostream>

namespace rat {
    ImageWidget::ImageWidget(const std::string& path) :
    Widget() {
        _texture.loadFromFile(path);
        _sprite.setTexture(_texture);
    }

    sf::Vector2u ImageWidget::_getSize() const {
        return _texture.getSize();
    }

    void ImageWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if(isActivated()) 
            target.draw(_sprite, states);
        
    }
}