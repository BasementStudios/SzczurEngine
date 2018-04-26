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
    }
    
    void NinePatch::draw(sf::RenderTarget& target)
    {
        if(_texture)
        {
            target.draw(_topLeftCorner);
            target.draw(_topBar);
            target.draw(_topRightCorner);

            target.draw(_leftBar);
            target.draw(_rightBar);
            target.draw(_bottomLeftCorner);
            target.draw(_bottomBar);
            target.draw(_bottomRightCorner);
        }
    }

    void NinePatch::setTexture(sf::Texture* texture)
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

        _leftBar.setTextureRect({ {0, innerY}, {innerX, innerH} });
        _rightBar.setTextureRect({ {innerX + innerW, innerY}, {textureW - (innerX + innerW), innerH} });

        _bottomLeftCorner.setTextureRect({ {0, innerY + innerH}, {innerX, textureH - (innerY + innerH)} });
        _bottomBar.setTextureRect({ {innerX, innerY + innerH}, {innerW, textureH - (innerY + innerH)} });
        _bottomRightCorner.setTextureRect({ {innerX + innerW, innerY + innerH}, {textureW - (innerX + innerW), textureH - (innerY + innerH)} });

        _recalcSpritesSize();
        _recalcSpritesPos();
    }

    void NinePatch::_recalcSpritesPos()
    {
        int leftCornerW = _topLeftCorner.getSize().x;
        int rightCornerW = _topLeftCorner.getSize().x;

        int topCornerH = _topLeftCorner.getSize().y;
        int bottomCornerH = _bottomLeftCorner.getSize().y;

        auto texSize = static_cast<sf::Vector2i>(_texture->getSize());
        int innerWidth = _size.x - (leftCornerW + rightCornerW);
        int innerHeight = _size.y - (topCornerH + bottomCornerH);

        _topLeftCorner.setPosition(_position);
        _topBar.setPosition(_position.x + leftCornerW, _position.y);
        _topRightCorner.setPosition(_position.x + leftCornerW + innerWidth, _position.y);

        _leftBar.setPosition(_position.x + 0, _position.y + topCornerH);
        _rightBar.setPosition(_position.x + leftCornerW + innerWidth, _position.y + topCornerH);

        _bottomLeftCorner.setPosition(_position.x + 0, _position.y + topCornerH + innerHeight);
        _bottomBar.setPosition(_position.x + leftCornerW, _position.y + topCornerH + innerHeight);
        _bottomRightCorner.setPosition(_position.x + leftCornerW + innerWidth, _position.y + topCornerH + innerHeight);
    }

    void NinePatch::_recalcSpritesSize()
    {
        int leftCornerW = _topLeftCorner.getSize().x;
        int rightCornerW = _topLeftCorner.getSize().x;

        int topCornerH = _topLeftCorner.getSize().y;
        int bottomCornerH = _bottomLeftCorner.getSize().y;

        auto texSize = static_cast<sf::Vector2i>(_texture->getSize());
        int innerWidth = _size.x - (leftCornerW + rightCornerW);
        int innerHeight = _size.y - (topCornerH + bottomCornerH);

        _topBar.setSize(innerWidth, 0);
        _bottomBar.setSize(innerWidth, 0);

        _leftBar.setSize(0, innerHeight);
        _rightBar.setSize(0, innerHeight);
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
    }

    void NinePatch::setPosition(float x, float y)
    {
        _position = {x, y};
        _recalcSpritesPos();
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

        _recalcSpritesSize();
        _recalcSpritesPos();
    }
    void NinePatch::setScale(float x, float y)
    {
        setScale({x, y});
    }
    
    
}