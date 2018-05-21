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

            for(int y = 0; y < _elementAmount.y; y++)
                for(int x = 0; x < _elementAmount.x; x++)
                {
                    _sprite.setPosition(float(x) * _elementDim.x, float(y) * _elementDim.y);
                    target.draw(_sprite, states);
                }
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
    const sf::Vector2i& Patch::getSize() const
    {
        return _size;
    }
    sf::Vector2i Patch::getElementSize() const
    {
        sf::Vector2f texSize = {(float)_sprite.getTextureRect().width, (float)_sprite.getTextureRect().height};
        return { int(texSize.x * _scale.x), int(texSize.y * _scale.y) };
    }
    
    
    void Patch::setTexture(const sf::Texture* texture)
    {
        _texture = nullptr;

        if(!_isTextureRectSet) setTextureRect({{0, 0}, static_cast<sf::Vector2i>(texture->getSize())});        
        if(!_isSizeSet) setSize( static_cast<sf::Vector2i>(texture->getSize()) );                        

        _texture = texture;
        _sprite.setTexture(*texture);

        _recalcRecurrence();
    }

    void Patch::setWidth(int width)
    {
        setSize(width, _size.y);
    }
    void Patch::setHeight(int height)
    {
        setSize(_size.x, height);
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

    void Patch::setDirection(Direction direction)
    {
        _direction = direction;
        if(_texture) _recalcRecurrence();
    }
    

    void Patch::_recalcRecurrence()
    {

        auto elementSize = static_cast<sf::Vector2f>(getElementSize());
        _elementDim = elementSize;
        _sprite.setScale(_scale);
        _elementAmount = {1, 1};


        if(_direction == Direction::Horizontal || _direction == Direction::All)
        {
            float totalWidth = float(_size.x);
            float realWidth = elementSize.x;
            
            float widthTimes = std::max(round(totalWidth/realWidth), 1.f);
            _elementAmount.x = int(widthTimes);
            _elementDim.x = totalWidth/widthTimes;

            float xScale = _elementDim.x / realWidth;
            _sprite.scale(xScale, 1.f);
        }
        if(_direction == Direction::Vertical || _direction == Direction::All)
        {
            float totalHeigt = float(_size.y);
            float realHeight = elementSize.y;
            
            float heightTimes = std::max(round(totalHeigt/realHeight), 1.f);
            _elementAmount.y = int(heightTimes);
            _elementDim.y = totalHeigt/heightTimes;

            float yScale = _elementDim.y / realHeight;
            _sprite.scale(1.f, yScale);
        }

        sf::Vector2f scale{1.f, 1.f};
        if(float(_size.x) < elementSize.x && _direction != Direction::Horizontal && _direction != Direction::All) scale.x = float(_size.x)/elementSize.x;
        if((float)_size.y < elementSize.y && _direction != Direction::Vertical && _direction != Direction::All) scale.y = float(_size.y)/elementSize.y;
        _sprite.scale(scale);
    }
    
}