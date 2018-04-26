#include "NinePatch.hpp"

#include <cmath>

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    NinePatch::NinePatch()
    {
        /*
        _sprites.clear();
        _sprites.emplace_back(&_topBar);
        _sprites.emplace_back(&_bottomBar);
        _sprites.emplace_back(&_leftBar);
        _sprites.emplace_back(&_rightBar);
        _sprites.emplace_back(&_topLeftCorner);
        _sprites.emplace_back(&_topRightCorner);
        _sprites.emplace_back(&_bottomLeftCorner);
        _sprites.emplace_back(&_bottomRightCorner);
        _sprites.emplace_back(&_inner);
        */
    }
    
    void NinePatch::draw(sf::RenderTarget& target)
    {
        if(_texture)
        {
            target.draw(_topLeftCorner);
            
            for(int i = 0; i < _innerWidthBarTimes; i++)
            {
                target.draw(_topBar);
                _topBar.move(_innerWidthBar, 0.f);
            }
            
            target.draw(_topRightCorner);  
            
            _recalcSpritesPos();          
        }
    }

    void NinePatch::setTexture(sf::Texture* texture)
    {
        _texture = texture;

        if(!_isInnerRectSet) _innerRect = {{0, 0}, static_cast<sf::Vector2i>(texture->getSize())};        
        if(!_isSizeSet) _size = static_cast<sf::Vector2i>(texture->getSize());                        

        _topLeftCorner.setTexture(*texture);
        _topBar.setTexture(*texture);
        _topRightCorner.setTexture(*texture);

        _recalcTexturesAreas();
    }
    void NinePatch::setTexture(sf::Texture* texture, const sf::IntRect& rect)
    {
        _texture = nullptr;
        setInnerSize(rect);
        setTexture(texture);
    }
    void NinePatch::setTexture(sf::Texture* texture, int paddingWidth, int paddingHeight)
    {
        auto textureSize = static_cast<sf::Vector2i>(texture->getSize());
        auto paddingSize = sf::Vector2i(2*paddingWidth, 2*paddingHeight);
        sf::IntRect rect = {{paddingWidth, paddingHeight}, textureSize - paddingSize};
        setTexture(texture, rect);
    }
    void NinePatch::setTexture(sf::Texture* texture, int padding)
    {
        setTexture(texture, padding, padding);
    }
    
    
    void NinePatch::setInnerSize(const sf::IntRect& rect)
    {
        _innerRect = rect;
        _isInnerRectSet = true;
        if(_texture) _recalcTexturesAreas();
    }
    void NinePatch::_recalcTexturesAreas()
    {
        if(!_texture) LOG_ERROR("Trying recalculating nullptr texture");

        int innerX = _innerRect.left;
        int innerY = _innerRect.top;
        int innerW = _innerRect.width;
        int innerH = _innerRect.height;

        int textureW = _texture->getSize().x;
        int textureH = _texture->getSize().y;

        _topLeftCorner.setTextureRect({ {0, 0}, {innerX, innerY} });
        _topBar.setTextureRect({ {innerX, 0}, {innerW, innerY} });
        _topRightCorner.setTextureRect({ {innerX + innerW, 0}, {textureW - (innerX + innerW), innerY} });

        _recalcSpritesPos();
        _recalcSpritesSize();
    }

    void NinePatch::_recalcSpritesPos()
    {
        int innerX = _innerRect.left;
        int innerY = _innerRect.top;
        int innerW = _innerRect.width;
        int innerH = _innerRect.height;

        auto texSize = static_cast<sf::Vector2i>(_texture->getSize());
        float innerWidth = float(_size.x - (texSize.x - _innerRect.width));
        float innerHeight = float(_size.y - (texSize.y - _innerRect.height));

        _topLeftCorner.setPosition(_position);
        _topBar.setPosition(_position.x + innerX, _position.y);
        _topRightCorner.setPosition(_position.x + innerX + innerWidth, _position.y);
    }

    void NinePatch::_recalcSpritesSize()
    {
        auto texSize = static_cast<sf::Vector2i>(_texture->getSize());
        float innerWidth = float(_size.x - (texSize.x - _innerRect.width));
        float innerHeight = float(_size.y - (texSize.y - _innerRect.height));


        float innerBarWidth = float(_topBar.getTextureRect().width);
        
        float widthTimes = round(innerWidth/innerBarWidth);
        _innerWidthBarTimes = int(widthTimes);
        _innerWidthBar = innerWidth/widthTimes;

        float xBarScale = _innerWidthBar / innerBarWidth;
        _topBar.setScale(xBarScale, 1.f);
    }
    
    void NinePatch::setInnerSize(int x, int y, int width, int height)
    {
        setInnerSize({{x, y}, {width, height}});
    }

    void NinePatch::setSize(int x, int y)
    {
        _isSizeSet = true;
        _size = {x, y};
        _recalcSpritesSize();
        _recalcSpritesPos();

        std::cout << "Width times: " << _innerWidthBarTimes << "\n";
    }

    void NinePatch::setPosition(float x, float y)
    {
        _position = {x, y};
        _recalcSpritesPos();
    }
    
    
}