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
                        _sprite.move(_elementDim.x, 0.f);
                    }
                } break;
                case Direction::Vertical:
                {
                    for(int i = 0; i < _elementAmount; i++)
                    {
                        target.draw(_sprite);
                        _sprite.move(0.f, _elementDim.y);
                    }
                } break;

                case Direction::None:
                {
                    target.draw(_sprite);
                } break;
            }

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

    void Patch::setDirection(Direction direction)
    {
        _direction = direction;
        if(_texture) _recalcRecurrence();
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
                _elementDim.x = int(totalWidth/widthTimes);

                float xScale = float(_elementDim.x) / realElementDim;
                _sprite.setScale(xScale * _scale.x, _scale.y);
            } break;

            case Direction::Vertical:
            {
                float totalHeigt = float(_size.y);

                float realElementDim = float(_sprite.getTextureRect().height) * _scale.y;
                
                float heightTimes = round(totalHeigt/realElementDim);
                _elementAmount = int(heightTimes);
                _elementDim.y = int(totalHeigt/heightTimes);

                float yScale = float(_elementDim.y) / realElementDim;
                _sprite.setScale(_scale.x, yScale * _scale.y);
            } break;

            case Direction::None:
            {
                _elementAmount = 1;
                auto texSize = static_cast<sf::Vector2f>(_texture->getSize());
                _elementDim.x = int(texSize.x * _scale.x);
                _elementDim.y = int(texSize.y * _scale.y);
                _sprite.setScale(_scale);
            } break;
        }
    }
    
}