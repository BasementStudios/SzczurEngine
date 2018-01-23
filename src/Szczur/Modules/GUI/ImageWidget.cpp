#include "ImageWidget.hpp"

#include <iostream>

namespace rat {
    ImageWidget::ImageWidget() :
    Widget() {

    }

    ImageWidget::ImageWidget(sf::Texture* texture) :
    Widget() {
        _sprite.setTexture(*texture);
    }

    void ImageWidget::setTexture(sf::Texture* texture) {
        _sprite.setTexture(*texture);
    }

    sf::Vector2u ImageWidget::_getSize() const {
        return _sprite.getTexture()->getSize();
    }

    void ImageWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(_sprite, states);
    }
}