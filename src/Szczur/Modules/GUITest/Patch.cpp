#include "Patch.hpp"

#include <cmath>

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    Patch::Patch(Direction direction)
    {
        setDirection(direction);
    } 

    void Patch::draw(sf::RenderTarget& target, sf::RenderStates states) const     
    {
        if(_texture)
        {        
            states.transform *= getTransform();
            switch(_direction)
            {
                case Direction::Horizontal:
                {
                    for(int i = 0; i < _elementAmount; i++)
                    {
                        target.draw(_sprite, states);
                        _sprite.move(_elementDim.x, 0.f);
                    }
                } break;
                case Direction::Vertical:
                {
                    for(int i = 0; i < _elementAmount; i++)
                    {
                        target.draw(_sprite, states);
                        _sprite.move(0.f, _elementDim.y);
                    }
                } break;

                case Direction::None:
                {
                    target.draw(_sprite, states);
                } break;
            }

           _sprite.setPosition(0.f, 0.f);
        }

    }
    
    void Patch::setScale(float x, float y)
    {
        setScale({x, y});
    }
    void Patch::setScale(const sf::Vector2f& scale)
    {
        _scale = scale;
        if(_texture) _recalcRecurrence();
    }
    const sf::Vector2i& Patch::getSize()
    {
        return _size;
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
        switch(_direction)
        {
            case Direction::Horizontal:
                _size.x = x;
            break;
            case Direction::Vertical:
                _size.y = y;
            break;
        }
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

    /*
    void Patch::setPosition(sf::Vector2f position)
    {
        setPosition(position.x, position.y);
    }
    
    void Patch::setPosition(float x, float y)
    {
        _position = {x, y};
        _recalcSpritePos();
    }
    void Patch::_recalcSpritePos() const
    {
        _sprite.setPosition(_position);
    }*/

    void Patch::setDirection(Direction direction)
    {
        _direction = direction;
        if(_texture) _recalcRecurrence();
    }
    /*
    const sf::Vector2f& Patch::getPosition()
    {
        return _position;
        sf::Transformable::
    }*/
    

    void Patch::_recalcRecurrence()
    {

        switch(_direction)
        {
            case Direction::Horizontal:
            {
                float totalWidth = float(_size.x);

                float realElementDim = float(_sprite.getTextureRect().width) * _scale.x;
                
                float widthTimes = std::max(round(totalWidth/realElementDim), 1.f);
                _elementAmount = int(widthTimes);
                _elementDim.x = totalWidth/widthTimes;

                float xScale = _elementDim.x / realElementDim;
                _sprite.setScale(xScale * _scale.x, _scale.y);
            } break;

            case Direction::Vertical:
            {
                float totalHeigt = float(_size.y);

                float realElementDim = float(_sprite.getTextureRect().height) * _scale.y;
                
                float heightTimes = std::max(round(totalHeigt/realElementDim), 1.f);
                _elementAmount = int(heightTimes);
                _elementDim.y = totalHeigt/heightTimes;

                float yScale = _elementDim.y / realElementDim;
                _sprite.setScale(_scale.x, yScale * _scale.y);
            } break;

            case Direction::None:
            {
                _elementAmount = 1;
                _sprite.setScale(_scale);
                _size = {int(_sprite.getGlobalBounds().width), int(_sprite.getGlobalBounds().height)};
            } break;
        }
    }
    
}