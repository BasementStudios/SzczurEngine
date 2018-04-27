#include "Scroller.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    void Scroller::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(_bar);
        target.draw(_scroller);
    }

    void Scroller::setPosition(float x, float y)
    {
        setPosition({x, y});
    }
    void Scroller::setPosition(const sf::Vector2f& pos)
    {
        auto shift = pos - _bar.getPosition();
        _bar.setPosition(pos);
        _setScrollerPosition(_scroller.getPosition() + shift);
    }
    

    void Scroller::setBarTexture(sf::Texture* texture)
    {
        _bar.setTexture(texture);
        _recalcOrigin();
        _recalcSize();
        _recalcPos();
    }
    void Scroller::setScrollerTexture(sf::Texture* texture)
    {
        _scroller.setTexture(*texture);
        _scroller.setScale(0.2f, 0.2f);
        _recalcOrigin();
        _recalcSize();
        _recalcPos();
        //_setScrollerPosition(_bar.getPosition() + sf::Vector2f{0.f, 150.f});
        _recalcRelPath();
        setProportion(0.5f);
    }

    
    void Scroller::_setScrollerPosition(float x, float y)
    {
        _setScrollerPosition({x, y});
    }
    void Scroller::_setScrollerPosition(const sf::Vector2f& pos)
    {
        _scroller.setPosition(pos);
        _recalcPos();
    }

    void Scroller::setSize(int x, int y)
    {
        setSize({x, y});
    }
    void Scroller::setSize(const sf::Vector2i& size)
    {
        _size = size;
        _bar.setSize(size);
        //_recalcSize();
        _recalcRelPath();
    }

    void Scroller::setScale(float x, float y)
    {
        setScale({x, y});
    }
    void Scroller::setScale(const sf::Vector2f& scale)
    {
        _bar.setScale(scale);
        _scroller.setScale(scale);
        _recalcOrigin();
    }

    void Scroller::_recalcSize()
    {
        _scrollerLength = _scroller.getGlobalBounds().height;
        _scrollerPathLength = _size.y - _scrollerLength;
    }
     void Scroller::_recalcPos()
     {
        auto barPos = _bar.getPosition();
        auto scrollerPos = _scroller.getPosition();

        auto scrollerWidth = _scroller.getGlobalBounds().width;
        _scroller.setPosition(barPos.x + float(scrollerWidth)/2.f, scrollerPos.y);       

        if(scrollerPos.y < barPos.y) _scroller.setPosition(scrollerPos.x, barPos.y);
        if(scrollerPos.y > barPos.y - _scrollerLength) _scroller.setPosition(scrollerPos.x, barPos.y - _scrollerLength);
     }
    
    void Scroller::_recalcOrigin()
    {
        auto scrollerWidth = _scroller.getTextureRect().width;
        _scroller.setOrigin(float(scrollerWidth)/2.f, 0.f); 
    }

    float Scroller::getProportion() const
    {
        if(_scrollerRelPath.y == 0.f) return 0.f;
        return _scrollerRelPos.y / _scrollerRelPath.y;
    }
    void Scroller::_recalcRelPath()
    {
        float oldProp = getProportion();
        
        auto path = _bar.getSize();
        std::cout << "Bar size: x: " << path.x << " y: " << path.y << "\n";
        path.y -= _scroller.getGlobalBounds().height;
        _scrollerRelPath = static_cast<sf::Vector2f>(path);

        _recalcRelPos(oldProp);
    }
    void Scroller::_recalcRelPos(float prop)
    {
        float newY = _scrollerRelPath.y * prop;
        _scrollerRelPos.y = newY;
        sf::Vector2f newScrollerPos = _bar.getPosition();
        newScrollerPos.y += newY;
        _scroller.setPosition(newScrollerPos);
    }
    void Scroller::_recalcRelPos()
    {
        _recalcRelPos(getProportion());
    }

    void Scroller::setProportion(float proportion)
    {
        _recalcRelPos(proportion);
    }
    
}