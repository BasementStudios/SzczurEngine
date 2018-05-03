#include "WindowWidget.hpp"

namespace rat
{
    WindowWidget::WindowWidget()
    :
    Widget()
    {
        _ninePatch.setScale(0.2f, 0.2f);
    }
    
    void WindowWidget::setTexture(sf::Texture* texture, int padding)
    {
        _ninePatch.setTexture(texture, padding);
        setPadding(padding, padding);
    }
    void WindowWidget::setScale(const sf::Vector2f& scale)
    {
        auto innerRect = _ninePatch.getInnerTextureRect();
        sf::Vector2f rectPadding = {float(innerRect.left), float(innerRect.top)};
        sf::Vector2f padding = {rectPadding.x * scale.x, rectPadding.y * scale.y};

        _ninePatch.setScale(scale);
        setPadding(padding);
    }
    void WindowWidget::setScale(float x, float y)
    {
        setScale({x, y});
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