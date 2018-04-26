#include "Patch.hpp"

#include <cmath>

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    void Patch::_recalcTexturesAreas()
    {
        switch(_direction)
        {
            case Direction::Horizontal:
            {

            }
        }
    }

    void Patch::draw(sf::RenderTarget& target) const
    {
        if(_texture)
        {        
            target.draw(_sprite);
            switch(_direction)
            {
                case Direction::Horizontal:
                {
                    for(int i = 0; i < _elementAmount; i++)
                    {
                        target.draw(_sprite);
                        _sprite.move(_elementDim, 0.f);
                    }
                    std::cout << "Drawing, Element Amount: " << _elementAmount << "\n";
                    std::cout << "Sprite Scale" << _sprite.getScale().x << "\n";
                    //std::cout << "Sprite Scale" << _sprite.getScale().x << "\n";
                }
                break;
            }
            std::cout << "Sprite Scale" << _sprite.getScale().x << "\n";

            _recalcSpritePos();
        }

    }
    
    void Patch::setScale(float x, float y)
    {
        _scale = {x, y};
        if(_texture) _recalcRecurrence();
    }
    void Patch::setTexture(sf::Texture* texture)
    {
        _texture = nullptr;

        if(!_isTextureRectSet) setTextureRect({{0, 0}, static_cast<sf::Vector2i>(texture->getSize())});        
        if(!_isSizeSet) setSize( static_cast<sf::Vector2i>(texture->getSize()) );                        

        _texture = texture;
        _sprite.setTexture(*texture);

        _recalcRecurrence();
    }
    
    void Patch::setSize(sf::Vector2i size)
    {
        setSize(size.x, size.y);
    }
    

    void Patch::setSize(int x, int y)
    {
        _size = {x, y};
        _isSizeSet = true;
        if(_texture) _recalcRecurrence();
    }
    
    void Patch::setTextureRect(const sf::IntRect& rect)
    {
        _sprite.setTextureRect(rect);
        _isTextureRectSet = true;
        if(_texture) _recalcRecurrence();
    }
    
    void Patch::_recalcSpriteSize()
    {}
    void Patch::setPosition(float x, float y)
    {
        _position = {x, y};
        _recalcSpritePos();
    }
    void Patch::_recalcSpritePos() const
    {
        _sprite.setPosition(_position);
    }

    void Patch::_recalcRecurrence()
    {

        switch(_direction)
        {
            case Direction::Horizontal:
            {
                float totalWidth = float(_size.x);

                float realElementDim = float(_sprite.getTextureRect().width) * _scale.x;
                
                float widthTimes = round(totalWidth/realElementDim);
                _elementAmount = int(widthTimes);
                _elementDim = totalWidth/widthTimes;

                float xScale = _elementDim / realElementDim;
                _sprite.setScale(xScale * _scale.x, _scale.y);

                std::cout << "Element Dim: " << _elementDim << "\n";
                std::cout << "Element Amount: " << _elementAmount << "\n";
                std::cout << "Element Scale: " << xScale * _scale.x << "\n";
                if(_texture)
                {
                    std::cout << "Texture size: x: " << _texture->getSize().x << "\n";
                }
            }
        }
    }
    
}