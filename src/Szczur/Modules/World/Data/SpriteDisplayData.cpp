#include "SpriteDisplayData.hpp"

#include <string>
#include <stdexcept>

#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RenderStates.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat {
    SpriteDisplayData::SpriteDisplayData(const std::string& name) 
        : _name(name) { 
    }

    void SpriteDisplayData::loadTexture() {
        try {
            _texture.loadFromFile(_name);
            _sprite.setTexture(_texture);
        }
        catch (const std::exception& e) {
            LOG_INFO(e.what());
        }
    }

    void SpriteDisplayData::loadTextureWithoutSet() {
        try {
            _texture.loadFromFile(_name);
        }
        catch(const std::exception& e) {
            LOG_INFO(e.what());
        }
    }

    void SpriteDisplayData::setupSprite() {
        _sprite.setTexture(_texture);
    }

    const sf3d::Texture& SpriteDisplayData::getTexture() const {    
        return _texture;
    }

    const std::string& SpriteDisplayData::getName() const {
        return _name;
    }

    void SpriteDisplayData::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const {
        target.draw(_sprite, states);
    }
}
