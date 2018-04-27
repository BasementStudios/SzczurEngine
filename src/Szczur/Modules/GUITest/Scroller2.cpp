#include "Scroller2.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    void Scroller2::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(_path);
        target.draw(_scroller);
        target.draw(_upperBound);
        target.draw(_bottomBound);
    }

    void Scroller2::setPosition(float x, float y)
    {
        _position = {x, y};
       _recalcAll();
    }
    void Scroller2::setPosition(const sf::Vector2f& position)
    {
        setPosition(position.x, position.y);
    }
    

    void Scroller2::setPathTexture(const sf::Texture* texture)
    {
        _isPathSet = true;
        _path.setTexture(*texture);
        _recalcAll();
    }
    void Scroller2::setScrollerTexture(const sf::Texture* texture)
    {
        _isScrollerSet = true;
        _scroller.setTexture(*texture);
        _recalcAll();
    }

    void Scroller2::setBoundTexture(const sf::Texture* texture)
    {
        _areBoundsSet = true;
        _upperBound.setTexture(*texture);
        _bottomBound.setTexture(*texture);
        _recalcAll();
    }
    
    
    void Scroller2::setProportion(float proportion)
    {
        if(proportion < 0.f) proportion = 0.f;
        if(proportion > 1.f) proportion = 1.f;
        _proportion = proportion;
        if(_isScrollerSet) _recalcScrollerPosByProp();
    }

    void Scroller2::setSize(const sf::Vector2i& size)
    {
        _size = size;
        _recalcAll();
    }
    void Scroller2::setWidthProportion(float proportion)
    {
        _widthProp = proportion;
        _recalcAll();
    }

    void Scroller2::setBoundShiftProportion(float proportion)
    {
        if(proportion < 0.f) proportion = 0.f;
        if(proportion > 1.f) proportion = 1.f;
        _boundShiftProp = proportion;
        _recalcAll();
    }
    

    void Scroller2::_recalcPathSize()
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
    void Scroller2::_recalcPathPos()
    {
        float pathWidth = _path.getGlobalBounds().width;
        float width = float(_size.x);

        float xShift = (width - pathWidth) / 2.f;
        float newX = _position.x + xShift;

        _path.setPosition(newX, _position.y + _getRealBoundLength());
    }

    void Scroller2::_recalcScrollerSize()
    {
        sf::Vector2f newScale = _scroller.getScale();
        auto texSize = sf::Vector2f{float(_scroller.getTextureRect().width), float(_scroller.getTextureRect().height)};
        newScale.x = float(_size.x)/texSize.x;
        if(_widthProp < 1.f) 
        {
            newScale.x *= _widthProp;
        }
        newScale.y = float(_scrollerLength) / texSize.y;

        _scroller.setScale(newScale);
    }
    
    void Scroller2::_recalcScrollerPos()
    {
        auto scrollerPos = _scroller.getPosition();
        float newX = scrollerPos.x;
        float newY = scrollerPos.y;

        auto realPathLength = _getRealPathLength();
        float maxTop = _path.getGlobalBounds().top;
        float maxBottom = maxTop + realPathLength;

        if(newY < maxTop) newY = maxTop;
        if(newY > maxBottom) newY = maxBottom;

        float scrollerWidth = _scroller.getGlobalBounds().width;
        float width = float(_size.x);

        float xShift = (width - scrollerWidth) / 2.f;
        newX = _position.x + xShift;

        _scroller.setPosition(newX, newY);

        _proportion = (maxBottom - scrollerPos.y) / realPathLength;
    }
    
    void Scroller2::_recalcScrollerPosByProp()
    {
        auto scrollerPos = _scroller.getPosition();
        auto realPathLength = _getRealPathLength();
        float maxTop = _path.getGlobalBounds().top;

        _scroller.setPosition(scrollerPos.x, maxTop + (realPathLength * _proportion));
    }

    void Scroller2::_recalcBoundPos()
    {
        _upperBound.setPosition(_position);
        float boundHeight = _bottomBound.getGlobalBounds().height;
        _bottomBound.setPosition(_position.x, _position.y + float(_size.y));
    }
    void Scroller2::_recalcBoundSize()
    {
        auto texSize = sf::Vector2f{float(_upperBound.getTextureRect().width), float(_upperBound.getTextureRect().height)};
        float scale = float(_size.x) / texSize.x;
        _upperBound.setScale(scale, scale);
        _bottomBound.setScale(scale, -scale);
    }
    
    float Scroller2::_getRealPathLength() const
    {
        //if(!_isPathSet) return 0.f;
        return _path.getGlobalBounds().height - _scroller.getGlobalBounds().height;
    }
    float Scroller2::_getRealBoundLength() const
    {
        if(!_areBoundsSet) return 0.f;
        return (_upperBound.getGlobalBounds().height * _boundShiftProp);
    }

    void Scroller2::_recalcAll()
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