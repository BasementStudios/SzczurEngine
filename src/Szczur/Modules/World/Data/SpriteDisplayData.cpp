#include "SpriteDisplayData.hpp"

namespace rat {
    SpriteDisplayData::SpriteDisplayData(const std::string& name) 
        : _name(name) { 
    }

    void SpriteDisplayData::loadTexture() {
        try {
            if(_texture.loadFromFile(_name)) {
                _sprite.setTexture(&_texture);
            }
        }
        catch(std::exception e) {
            LOG_INFO(e.what());
        }
    }

    void SpriteDisplayData::loadTextureWithoutSet() {
        try {
            _texture.loadFromFile(_name);
        }
        catch(std::exception e) {
            LOG_INFO(e.what());
        }
    }

    void SpriteDisplayData::setupSprite() {
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