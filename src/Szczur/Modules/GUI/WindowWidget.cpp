#include "WindowWidget.hpp"

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
    
    void WindowWidget::setScale(float x, float y)
    {
        setScale({x, y});
    }

    void WindowWidget::setPadding(const sf::Vector2f& padding)
    {
        _isPaddingSet = true;
        Widget::setPadding(padding);
    }

    void WindowWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(_ninePatch, states);
    }
	sf::Vector2u WindowWidget::_getSize() const
    {
        return {};
    
    }
	void WindowWidget::_calculateSize()
    {
        auto size = static_cast<sf::Vector2i>(getSize());
        _ninePatch.setSize(size);
    }
}