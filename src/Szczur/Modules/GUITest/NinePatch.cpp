#include "NinePatch.hpp"

#include <cmath>

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    NinePatch::NinePatch()
    {
        _topBar.setDirection(Patch::Direction::Horizontal);
        _bottomBar.setDirection(Patch::Direction::Horizontal);

        _leftBar.setDirection(Patch::Direction::Vertical);
        _rightBar.setDirection(Patch::Direction::Vertical);

        _topLeftCorner.setDirection(Patch::Direction::None);
        _topRightCorner.setDirection(Patch::Direction::None);
        _bottomLeftCorner.setDirection(Patch::Direction::None);
        _bottomRightCorner.setDirection(Patch::Direction::None);

        _inner.setDirection(Patch::Direction::All);
    }
    
    void NinePatch::draw(sf::RenderTarget& target, sf::RenderStates states) const 
    {
        if(_texture)
        {
            states.transform *= getTransform();

            target.draw(_topLeftCorner, states);
            target.draw(_topBar, states);
            target.draw(_topRightCorner, states);

            target.draw(_leftBar, states);
            target.draw(_rightBar, states);
            target.draw(_bottomLeftCorner, states);
            target.draw(_bottomBar, states);
            target.draw(_bottomRightCorner, states);

            target.draw(_inner, states);
        }
    }

    void NinePatch::setTexture(const sf::Texture* texture)
    {
        _texture = texture;

        _topBar.setTexture(texture);
        _bottomBar.setTexture(texture);
        _leftBar.setTexture(texture);
        _rightBar.setTexture(texture);
        _topLeftCorner.setTexture(texture);
        _topRightCorner.setTexture(texture);
        _bottomLeftCorner.setTexture(texture);
        _bottomRightCorner.setTexture(texture);
        _inner.setTexture(texture);

        _recalcTexturesAreas();
    }
    void NinePatch::setTexture(const sf::Texture* texture, const sf::IntRect& rect)
    {
        _texture = nullptr;
        setInnerSize(rect);
        setTexture(texture);
    }
    void NinePatch::setTexture(const sf::Texture* texture, int paddingWidth, int paddingHeight)
    {
        auto textureSize = static_cast<sf::Vector2i>(texture->getSize());
        auto paddingSize = sf::Vector2i(2*paddingWidth, 2*paddingHeight);
        sf::IntRect rect = {{paddingWidth, paddingHeight}, textureSize - paddingSize};
        setTexture(texture, rect);
    }
    void NinePatch::setTexture(const sf::Texture* texture, int padding)
    {
        setTexture(texture, padding, padding);
    }
    
    
    void NinePatch::setInnerSize(const sf::IntRect& rect)
    {
        _innerRect = rect;
        _isInnerRectSet = true;
        if(_texture) _recalcTexturesAreas();
    }
    void NinePatch::setInnerSize(int x, int y, int width, int height)
    {
        setInnerSize({{x, y}, {width, height}});
    }

    void NinePatch::setColor(const sf::Color& color)
    {
        _topBar.setColor(color);
        _bottomBar.setColor(color);
        _leftBar.setColor(color);
        _rightBar.setColor(color);

        _topLeftCorner.setColor(color);
        _topRightCorner.setColor(color);
        _bottomLeftCorner.setColor(color);
        _bottomRightCorner.setColor(color);

        _inner.setColor(color);
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

        _leftBar.setTextureRect({ {0, innerY}, {innerX, innerH} });
        _rightBar.setTextureRect({ {innerX + innerW, innerY}, {textureW - (innerX + innerW), innerH} });

        _bottomLeftCorner.setTextureRect({ {0, innerY + innerH}, {innerX, textureH - (innerY + innerH)} });
        _bottomBar.setTextureRect({ {innerX, innerY + innerH}, {innerW, textureH - (innerY + innerH)} });
        _bottomRightCorner.setTextureRect({ {innerX + innerW, innerY + innerH}, {textureW - (innerX + innerW), textureH - (innerY + innerH)} });

        _inner.setTextureRect({ {innerX, innerY}, {innerW, innerH} });

        _recalcSpritesSize();
        _recalcSpritesPos();
    }

    void NinePatch::_recalcSpritesPos()
    {
        int leftCornerW = _topLeftCorner.getSize().x;
        int rightCornerW = _topLeftCorner.getSize().x;

        int topCornerH = _topLeftCorner.getSize().y;
        int bottomCornerH = _bottomLeftCorner.getSize().y;

        int innerWidth = _size.x - (leftCornerW + rightCornerW);
        int innerHeight = _size.y - (topCornerH + bottomCornerH);

        _topLeftCorner.setPosition({0.f, 0.f});
        _topBar.setPosition(leftCornerW, 0.f);
        _topRightCorner.setPosition(leftCornerW + innerWidth, 0.f);

        _leftBar.setPosition(0, topCornerH);
        _rightBar.setPosition(leftCornerW + innerWidth, topCornerH);

        _bottomLeftCorner.setPosition(0, topCornerH + innerHeight);
        _bottomBar.setPosition(leftCornerW, topCornerH + innerHeight);
        _bottomRightCorner.setPosition(leftCornerW + innerWidth, topCornerH + innerHeight);

        _inner.setPosition(leftCornerW, topCornerH);
    }

    void NinePatch::_recalcSpritesSize()
    {
        int width = _size.x;
        int height = _size.y;


        int leftCornerW = _topLeftCorner.getElementSize().x;
        int rightCornerW = _topRightCorner.getElementSize().x;

        int topCornerH = _topLeftCorner.getElementSize().y;
        int bottomCornerH = _bottomLeftCorner.getElementSize().y;

        int cornersWidth = leftCornerW + rightCornerW;
        int cornersHeight = topCornerH + bottomCornerH;

        int innerWidth = width - cornersWidth;
        int innerHeight = height - cornersHeight;

        if(innerWidth < 0)
        {
            float prop = float(leftCornerW)/float(cornersWidth);

            int leftW = int(round(prop * float(_size.x)));
            int rightW = int(round((1.f - prop) * float(_size.x)));

            _topLeftCorner.setWidth(leftW);
            _bottomLeftCorner.setWidth(leftW);
            _leftBar.setWidth(leftW);

            _topRightCorner.setWidth(rightW);
            _bottomRightCorner.setWidth(rightW);
            _rightBar.setWidth(rightW);

            

            innerWidth = 0;
        }
        else
        {
            _topLeftCorner.setWidth(leftCornerW);
            _bottomLeftCorner.setWidth(leftCornerW);
            _leftBar.setWidth(leftCornerW);

            _topRightCorner.setWidth(rightCornerW);
            _bottomRightCorner.setWidth(rightCornerW);
            _rightBar.setWidth(rightCornerW);
        }
        if(innerHeight < 0)
        {
            float prop = float(topCornerH)/float(cornersHeight);
            int topH = int(round(prop * float(_size.y)));
            int bottomH = int(round((1.f - prop) * float(_size.y)));

            _topLeftCorner.setHeight(topH);
            _topRightCorner.setHeight(topH);
            _topBar.setHeight(topH);

            _bottomLeftCorner.setHeight(bottomH);
            _bottomRightCorner.setHeight(bottomH);
            _bottomBar.setHeight(bottomH);

            innerHeight = 0;
        }
        else
        {
            _topLeftCorner.setHeight(topCornerH);
            _topRightCorner.setHeight(topCornerH);
            _topBar.setHeight(topCornerH);            

            _bottomLeftCorner.setHeight(bottomCornerH);
            _bottomRightCorner.setHeight(bottomCornerH);
            _bottomBar.setHeight(bottomCornerH);
        }

        _topBar.setWidth(innerWidth);
        _bottomBar.setWidth(innerWidth);

        _leftBar.setHeight(innerHeight);
        _rightBar.setHeight(innerHeight);

        _inner.setSize(innerWidth, innerHeight);


    }
    

    void NinePatch::setSize(const sf::Vector2i& size)
    {
        _isSizeSet = true;
        _size = size;
        _recalcSpritesSize();
        _recalcSpritesPos();
    }

    void NinePatch::setSize(int x, int y)
    {
        setSize({x, y});
    }

    sf::Vector2i NinePatch::getSize() const
    {
        return _size;
    }

    void NinePatch::setScale(const sf::Vector2f& scale)
    {
        _topLeftCorner.setScale(scale);
        _topBar.setScale(scale);
        _topRightCorner.setScale(scale);
        _leftBar.setScale(scale);
        _rightBar.setScale(scale);
        _bottomLeftCorner.setScale(scale);
        _bottomBar.setScale(scale);
        _bottomRightCorner.setScale(scale);
        _inner.setScale(scale);

        _recalcSpritesSize();
        _recalcSpritesPos();
    }
    
    void NinePatch::setScale(float x, float y)
    {
        setScale({x, y});
    }

    sf::Vector2i NinePatch::getInnerPathSize() const
    {
        return _inner.getElementSize();
    }
    sf::Vector2i NinePatch::getInnerSize() const
    {
        return _inner.getSize();
    }

    sf::Vector2i NinePatch::getCornersCombinedSize() const
    {
        int width = _topLeftCorner.getElementSize().x + _topRightCorner.getElementSize().x;
        int height = _topLeftCorner.getElementSize().y + _bottomLeftCorner.getElementSize().y;
        return {width, height};
    }

    sf::IntRect NinePatch::getInnerTextureRect() const
    {
        return _innerRect;
    }
    
    
    
}