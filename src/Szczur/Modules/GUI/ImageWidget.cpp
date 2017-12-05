#include "ImageWidget.hpp"

#include <iostream>

namespace rat {
    ImageWidget::ImageWidget() :
    Widget() {

    }

    ImageWidget::ImageWidget(const std::string& path) :
    Widget() {
        _texture.loadFromFile(path);
        _sprite.setTexture(_texture);
    }

    void ImageWidget::setTexture(const std::string &path) {
        _texture.loadFromFile(path);
        _sprite.setTexture(_texture);
    }

    sf::Vector2u ImageWidget::_getSize() const {
        return _texture.getSize();
    }

    void ImageWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(_sprite, states);
    }
}