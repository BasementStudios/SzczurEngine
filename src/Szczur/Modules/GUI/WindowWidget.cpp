#include "WindowWidget.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    WindowWidget::WindowWidget()
    :
    Widget()
    {
        setScale({0.2f, 0.2f});
    }
    
    void WindowWidget::setTexture(sf::Texture* texture, int padding)
    {
        _ninePatch.setTexture(texture, padding);
        _calcPadding();
    }
    void WindowWidget::setScale(const sf::Vector2f& scale)
    {
        _ninePatch.setScale(scale);
        _scale = scale;
        _calcPadding();
    }
    void WindowWidget::setScale(float x, float y)
    {
        setScale({x, y});
    }

    void WindowWidget::setPadding(const sf::Vector2f& padding)
    {
        _isPaddingSet = true;
        Widget::setPadding(padding);
    }
    void WindowWidget::setPadding(float x, float y)
    {
        setPadding({x, y});
    }
    void WindowWidget::_calcPadding()
    {
        if(!_isPaddingSet)
        {
            auto innerRect = _ninePatch.getInnerTextureRect();
            sf::Vector2f rectPadding = {float(innerRect.left), float(innerRect.top)};
            sf::Vector2f padding = {rectPadding.x * _scale.x, rectPadding.y * _scale.y};
            Widget::setPadding(padding);
        }
    }
    


    void WindowWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(_ninePatch, states);
    }
	sf::Vector2u WindowWidget::_getSize() const
    {
        return _minWinSize;
    }
    
    void WindowWidget::setPatchAmount(const sf::Vector2u& amount)
    {
        _isPathesAmountSet = true;
        _patchesAmount = amount;
        _calcPatchesAmount();
        _aboutToRecalculate = true;
    }
    void WindowWidget::setPatchAmount(unsigned int horizontalAmount, unsigned int verticalAmount)
    {
        setPatchAmount({horizontalAmount, verticalAmount});
    }
	void WindowWidget::_calculateSize()
    {
        _calcPatchesAmount();
        auto size = static_cast<sf::Vector2u>(getSize());
        size.x = std::max(_minWinSize.x, size.x);
        size.y = std::max(_minWinSize.y, size.y);
        _ninePatch.setSize(static_cast<sf::Vector2i>(size));
    }
    void WindowWidget::_calcPatchesAmount()
    {
        if(_isPathesAmountSet)
        {
            auto innerElSize = _ninePatch.getInnerPathSize();
            sf::Vector2i innerSize = {innerElSize.x * int(_patchesAmount.x), innerElSize.y * int(_patchesAmount.y)};
            auto newSize = innerSize + _ninePatch.getCornersCombinedSize();
            _minWinSize = static_cast<sf::Vector2u>(newSize);
        }
    }


}