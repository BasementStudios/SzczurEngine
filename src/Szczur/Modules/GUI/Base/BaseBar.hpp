#pragma once

#include <memory>

#include <SFML/System/Vector2.hpp>

namespace sf { class Texture; class Font;}

namespace rat
{
    class GUI;
    class Widget; class ImageWidget; class WindowWidget; class TextWidget;
    class TextAreaWidget;

    class BaseBar
    {
    public:
        BaseBar();
        BaseBar(Widget* base);
        void setPosition(float x, float y);
        void setPosition(const sf::Vector2f& position);
        sf::Vector2f getPosition() const;
        void setSize(const sf::Vector2u& size);
        void setSize(unsigned int width, unsigned int height);
        sf::Vector2u getSize() const;
        void move(float offsetX, float offsetY);
        void move(const sf::Vector2f& offset);
        void setParent(Widget* parent);
        void deactivate();
        void activate();
        bool isActivate() const;
        void setPropOrigin(const sf::Vector2f& propOrigin);
        void setPropOrigin(float x, float y);
        void setPropSize(const sf::Vector2f& propSize);
        void setPropSize(float propWidth, float propHeight);
        void setPropPosition(const sf::Vector2f& propPos);
        void setPropPosition(float propX, float propY);


    protected:
        void _addWidget(Widget* addon);
        void _setWidgetSize(ImageWidget* widget, float x, float y);
        void _addBar(BaseBar& addon);
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