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
            const auto oldPos = _sprite.getPosition();
            for(int y = 0; y < _elementAmount.y; y++)
                for(int x = 0; x < _elementAmount.x; x++)
                {
                    _sprite.setPosition(oldPos + sf::Vector2f{float(x) * _elementDim.x, float(y) * _elementDim.y});
                    target.draw(_sprite, states);
                }
            _sprite.setPosition(oldPos);
        }
    }

    void Patch::setBasePosition(const sf::Vector2f& basePosition)
    {
        _sprite.move(-_basePos);
        _basePos = basePosition;
        _sprite.move(_basePos);

    }
    void Patch::setPosition(const sf::Vector2f& position)
    {
        _sprite.setPosition(_basePos + position);
    }
    void Patch::setPosition(float x, float y)
    {
        setPosition({x, y});
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
    const sf::Vector2f& Patch::getSize() const
    {
        return _size;
    }
    sf::Vector2f Patch::getElementSize() const
    {
        sf::Vector2f texSize = {(float)_sprite.getTextureRect().width, (float)_sprite.getTextureRect().height};
        return { texSize.x * _scale.x, texSize.y * _scale.y };
    }
    
    
    void Patch::setTexture(const sf::Texture* texture)
    {
        _texture = nullptr;

        if(!_isTextureRectSet) setTextureRect({{0, 0}, static_cast<sf::Vector2i>(texture->getSize())});        
        if(!_isSizeSet) setSize( static_cast<sf::Vector2f>(texture->getSize()) );                        

        _texture = texture;
        _sprite.setTexture(*texture);

        _recalcRecurrence();
    }

    void Patch::setWidth(float width)
    {
        setSize(width, _size.y);
    }
    void Patch::setHeight(float height)
    {
        setSize(_size.x, height);
    }
    
    void Patch::setSize(sf::Vector2f size)
    {
        setSize(size.x, size.y);
    }
    

    void Patch::setSize(float x, float y)
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

    void Patch::setColor(const sf::Color& color)
    {
        _sprite.setColor(color);
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
            
            float widthTimes = std::max(float(round(totalWidth/realWidth)), 1.f);
            _elementAmount.x = int(widthTimes);
            _elementDim.x = totalWidth/widthTimes;

            float xScale = _elementDim.x / realWidth;
            _sprite.scale(xScale, 1.f);
        }
        if(_direction == Direction::Vertical || _direction == Direction::All)
        {
            float totalHeigt = float(_size.y);
            float realHeight = elementSize.y;
            
            float heightTimes = std::max(float(round(totalHeigt/realHeight)), 1.f);
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
