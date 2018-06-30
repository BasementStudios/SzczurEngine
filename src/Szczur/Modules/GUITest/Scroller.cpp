#include "Scroller.hpp"

#include <cmath>
#include <iomanip>

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    void Scroller::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(_path, states);
        target.draw(_scroller, states);
        target.draw(_upperBound, states);
        target.draw(_bottomBound, states);
    }

    void Scroller::setPosition(float x, float y)
    {
        gui::FamilyTransform::setPosition(x, y);
        _position = {x, y};
       _recalcAll();
    }
    void Scroller::setPosition(const sf::Vector2f& position)
    {
        setPosition(position.x, position.y);
    }
    void Scroller::setScrollerPosition(const sf::Vector2f& position)
    {
        _scroller.setPosition(position + gui::FamilyTransform::getDrawPosition());
        _recalcScrollerPos();
    }
    void Scroller::setScrollerHeightProp(float prop)
    {
        if(prop < 1.f) prop = 1.f;
        _scrollerHeightProp = prop;
        if(_isScrollerSet)
        {
            _recalcScrollerSize();
            _recalcScrollerSize();
        }
    }

    sf::FloatRect Scroller::getBound() const
    {
        return {_position, _size};
    }
    

    void Scroller::applyFamilyTransform(const sf::Vector2f& globalPos, const sf::Vector2f& drawPos)
    {
        gui::FamilyTransform::applyParentPosition(globalPos, drawPos);

        _recalcAll();
    }
    
    

    void Scroller::setPathTexture(const sf::Texture* texture)
    {
        _isPathSet = true;
        _path.setTexture(*texture);
        _recalcAll();
    }
    void Scroller::setScrollerTexture(sf::Texture* texture, int boundHeight)
    {
        _isScrollerSet = true;
        auto texSize = texture->getSize();
        auto texHalfWidth = int(round(float(texSize.x) * 0.5f));
        _scroller.setTexture(texture, texHalfWidth, 139);
        _recalcAll();
    }

    void Scroller::setBoundTexture(const sf::Texture* texture)
    {
        _areBoundsSet = true;
        _upperBound.setTexture(*texture);
        _bottomBound.setTexture(*texture);
        _recalcAll();
    }
    
    
    void Scroller::setProportion(float proportion)
    {
        if(proportion < 0.f) proportion = 0.f;
        if(proportion > 1.f) proportion = 1.f;
        _proportion = proportion;
        if(_isScrollerSet) _recalcScrollerPosByProp();
    }

    void Scroller::setScrollerPropHeight(float propY)
    {
        if(propY < 1.f) propY = 1.f;
        _scrollerHeightProp = propY;
        _recalcScrollerSize();
        _recalcScrollerPosByProp();
    }

    void Scroller::moveProportion(float proportionOffset)
    {
        setProportion(_proportion + proportionOffset/_scrollerHeightProp);
    }
    float Scroller::getProportion() const
    {
        return _proportion;
    }
    
    

    void Scroller::setSize(float width, float height)
    {
        setSize({width, height});
    }
    
    void Scroller::setSize(const sf::Vector2f& size)
    {
        _size = size;
        _recalcAll();
    }
    void Scroller::setWidthProportion(float proportion)
    {
        _widthProp = proportion;
        _recalcAll();
    }

    void Scroller::setBoundShiftProportion(float proportion)
    {
        if(proportion < 0.f) proportion = 0.f;
        if(proportion > 1.f) proportion = 1.f;
        _boundShiftProp = proportion;
        _recalcAll();
    }

    sf::Vector2f Scroller::getSize() const
    {
        return sf::Vector2f(_size);
    }

    sf::Vector2f Scroller::getScrollerSize() const
    {
        return _scroller.getSize();
    }
    sf::Vector2f Scroller::getScrollerPosition() const
    {
        return _scroller.getPosition();
    }
    
    

    void Scroller::input(const sf::Event& event)
    {
        
        switch (event.type)
        {
            case sf::Event::MouseMoved:
            {
                const auto scPos = _getScrollerGlobalPos();
                const auto scSize = _scroller.getSize();

                const sf::Vector2f mousePos{float(event.mouseMove.x), float(event.mouseMove.y)};
                if(_hasBeenClicked)
                {
                    _clickedShift = mousePos - scPos;
                    _hasBeenClicked = false;
                }
                if(_isClicked)
                {
                    const auto newGlobalPos = mousePos - _clickedShift;
                    const auto newPos = newGlobalPos - (getGlobalPosition() - getDrawPosition()); 
                    //std::cout << std::setw(8) << "GlobalPos" << "x:" << getGlobalPosition().x << '\t' << getGlobalPosition().y << '\n' <<
                    //"DrawPos x: " << getDrawPosition().x << '\t' << getDrawPosition().y << '\n' <<
                    //"GlobalMousePos x: " << newGlobalPos.x << '\t' << newGlobalPos.y << '\n' <<
                    //"NewPos x: " << newPos.x << '\t' << newPos.y << '\n';
                    setScrollerPosition(newPos);
                }

                bool isMouseOverlap = mousePos.x >= scPos.x &&
                    mousePos.x <= scPos.x + scSize.x &&
                    mousePos.y >= scPos.y &&
                    mousePos.y <= scPos.y + scSize.y;

                if(isMouseOverlap)
                {
                    _isHovered = true;
                }
                else 
                {
                    _isHovered = false;
                }

            } break;

            case sf::Event::MouseButtonPressed:
            {
                if(_isHovered) 
                {
                    _isClicked = true;
                    _hasBeenClicked = true;
                }
            } break;

            case sf::Event::MouseButtonReleased:
            {
                _isClicked = false;
            }
        }
    }

    sf::Vector2f Scroller::_getScrollerGlobalPos() const
    {
        return _scroller.getPosition() - gui::FamilyTransform::getDrawPosition() + gui::FamilyTransform::getGlobalPosition();
    }

    void Scroller::_recalcPathSize()
    {
        float widthProp = 1.f/_widthProp;

        sf::Vector2f newScale = _path.getScale();
        auto texSize = sf::Vector2f{float(_path.getTextureRect().width), float(_path.getTextureRect().height)};
        newScale.x = float(_size.x)/texSize.x;
        if(widthProp < 1.f) 
        {
            newScale.x *= widthProp;
        }
        newScale.y = (float(_size.y) - 2.f * _getRealBoundLength()) / texSize.y;

        _path.setScale(newScale);
    }
    void Scroller::_recalcPathPos()
    {
        float pathWidth = _path.getGlobalBounds().width;
        float width = float(_size.x);

        float xShift = (width - pathWidth) / 2.f;
        float newX = xShift;

        _path.setPosition(sf::Vector2f{newX, _getRealBoundLength()} + gui::FamilyTransform::getDrawPosition());
    }

    void Scroller::_recalcScrollerSize()
    {
        auto innerTexSize = _scroller.getInnerTextureRect();
        sf::Vector2f texSize = {float(innerTexSize.left * 2), float(innerTexSize.top * 2)};
        float scale = float(_size.x) / float(texSize.x);
        _scroller.setScale(scale, scale);

        auto newSize = _size;
        if(_widthProp < 1.f) 
        {
            newSize.x = _size.x * _widthProp;
        }
        newSize.y = _path.getGlobalBounds().height / _scrollerHeightProp;
        _scroller.setSize(newSize);

    }
    
    void Scroller::_recalcScrollerPos()
    {
        auto scrollerPos = _scroller.getPosition();
        float newX = scrollerPos.x;
        float newY = scrollerPos.y;

        auto realPathLength = _getRealPathLength();
        float maxTop = _path.getGlobalBounds().top;
        float maxBottom = maxTop + realPathLength;

        const auto drawPos = gui::FamilyTransform::getDrawPosition();

        //std::cout << "MaxTop: " << maxTop << " maxBottom: " << maxBottom << '\n';

        if(newY < maxTop) newY = maxTop;
        if(newY > maxBottom) newY = maxBottom;

        float scrollerWidth = _scroller.getSize().x;
        float width = _size.x;

        float xShift = (width - scrollerWidth) / 2.f;
        newX = xShift + drawPos.x;

        _scroller.setPosition(newX, newY);

        float prop = (newY - maxTop) / realPathLength;

        _proportion = std::max(0.f, std::min(1.f, prop));
    }
    
    void Scroller::_recalcScrollerPosByProp()
    {
        auto realPathLength = _getRealPathLength();
        float maxTop = _path.getGlobalBounds().top;

        _scroller.setPosition(0.f, maxTop + (realPathLength * _proportion));
    }

    void Scroller::_recalcBoundPos()
    {
        float boundHeight = _bottomBound.getGlobalBounds().height;
        _upperBound.setPosition(gui::FamilyTransform::getDrawPosition());
        _bottomBound.setPosition(sf::Vector2f{0.f, float(_size.y)} + gui::FamilyTransform::getDrawPosition());
    }
    void Scroller::_recalcBoundSize()
    {
        auto texSize = sf::Vector2f{float(_upperBound.getTextureRect().width), float(_upperBound.getTextureRect().height)};
        float scale = float(_size.x) / texSize.x;
        _upperBound.setScale(scale, scale);
        _bottomBound.setScale(scale, -scale);
    }
    
    float Scroller::_getRealPathLength() const
    {
        //if(!_isPathSet) return 0.f;
        auto length =  _path.getGlobalBounds().height - _scroller.getSize().y;
        if(length < 0.f) length = 0.f;
        return length;
    }
    float Scroller::_getRealBoundLength() const
    {
        if(!_areBoundsSet) return 0.f;
        return (_upperBound.getGlobalBounds().height * _boundShiftProp);
    }

    void Scroller::_recalcAll()
    {
        if(_areBoundsSet)
        {
            _recalcBoundSize();
            _recalcBoundPos();
        }
        if(_isPathSet)
        {
            _recalcPathSize();
            _recalcScrollerSize();
        }
        if(_isScrollerSet)
        {
            _recalcScrollerPosByProp();
            _recalcScrollerPos();
            _recalcPathPos();
        }
    }
    
}