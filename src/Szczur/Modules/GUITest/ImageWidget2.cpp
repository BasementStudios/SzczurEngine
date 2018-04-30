#include "ImageWidget2.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    void ImageWidget2::setTexture(const sf::Texture* texture)
    {
        _texture = texture;
        _sprite.setTexture(*_texture);
        _reacalcSpriteSize();
        _recalcSize();
    }
    
    void ImageWidget2::_recalculateComponentsPosition()
    {
        auto position = getPosition();
        _sprite.setPosition(position);
    }
    sf::Vector2f ImageWidget2::_getComponentsSize() const
    {
        if(hasMinimalSize()) return static_cast<sf::Vector2f>(getMinimalSize());
        if(!_texture) return {};
        return static_cast<sf::Vector2f>(_texture->getSize());
    }
    void ImageWidget2::_recalculateComponentsSize()
    {
        _reacalcSpriteSize();
    }
    void ImageWidget2::_drawComponents(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(_sprite);
    }
    void ImageWidget2::_reacalcSpriteSize()
    {
        if(!_texture) return;
        if(!hasMinimalSize()) _sprite.setScale(1.f, 1.f);
        else
        {
            auto size = static_cast<sf::Vector2f>(getMinimalSize());
            auto texSize = static_cast<sf::Vector2f>(_texture->getSize());
            sf::Vector2f scale = {size.x/texSize.x, size.y/texSize.y};
            _sprite.setScale(scale);
        }
    }

    
}