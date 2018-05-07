#include "SpriteDisplayData.hpp"

namespace rat {
    SpriteDisplayData::SpriteDisplayData(const std::string& name) :
    _name(name) {
        if (!this->_texture.loadFromFile(_assetsFolderPath + name + _textureFilePath))
			throw std::runtime_error("Cannot load texture file.");
        _sprite.setTexture(&_texture);
    }

    const sf3d::Texture& SpriteDisplayData::getTexture() const {
        return _texture;
    }

    const std::string& SpriteDisplayData::getName() const {
        return _name;
    }

    void SpriteDisplayData::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const {
        states.texture = &_texture;
        target.draw(_sprite, states);
    }
}