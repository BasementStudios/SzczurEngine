#pragma once

#include <memory>

#include <SFML/System/Vector2.hpp>

namespace rat
{
    class Widget; class ImageWidget; class WindowWidget; class TextWidget;
    class TextAreaWidget; 

    class BaseBar
    {
    public:
        BaseBar();
        void setPosition(float x, float y);
        void setPosition(const sf::Vector2f& position);
        void setSize(const sf::Vector2u& size);
        void setSize(unsigned int width, unsigned int height);
        void move(float offsetX, float offsetY);
        void move(const sf::Vector2f& offset);
        void setParent(Widget* parent);
        void deactivate();
        void activate();
        bool isActivate() const;
        void setPropOrigin(const sf::Vector2f& propOrigin);
        void setPropOrigin(float x, float y);

    protected:
        void _addWidget(Widget* addon);
        void _setWidgetSize(ImageWidget* widget, float x, float y);
        template<typename B>
        void _addBar(std::unique_ptr<B>& bar);
    private:
        Widget* _base{nullptr};

    };

    template<typename B>
    void BaseBar::_addBar(std::unique_ptr<B>& bar)
    {
        bar->setParent(_base);
    }
}