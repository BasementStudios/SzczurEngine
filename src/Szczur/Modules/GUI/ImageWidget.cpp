#include "ImageWidget.hpp"

namespace rat {
    ImageWidget::ImageWidget(const std::string& fileName) {
        _texture.loadFromFile(fileName);
        _sprite.setTexture(_texture);
    }

    bool ImageWidget::_input(const sf::Event& event) {

    }

	void ImageWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(_sprite, states);
    }

	void ImageWidget::_update(float deltaTime) {

    }

	sf::Vector2u ImageWidget::_getSize() {
        return _texture.getSize();
    }

}