#include "ButtonWidget.hpp"

#include <iostream>

namespace rat {
    ButtonWidget::ButtonWidget(Widget* parent, std::function<bool(Widget*)> callback) :
        Widget(parent),
        _isHovered(false),
        _callbackWithWidget(true) {
        _callback = callback;
    }

    ButtonWidget::ButtonWidget(Widget* parent, std::function<bool()> callback) :
        Widget(parent),
        _isHovered(false),
        _callbackWithWidget(false) {
        _callback = callback;
    }

    ButtonWidget* ButtonWidget::setCallback(std::function<bool(Widget*)> callback) {
        _callback = callback;
        _callbackWithWidget = true;
    }

    ButtonWidget* ButtonWidget::setCallback(std::function<bool()> callback) {
        _callback = callback;
        _callbackWithWidget = false;
    }


    bool ButtonWidget::_input(const sf::Event& event) {
        switch(event.type) {
            case sf::Event::MouseButtonPressed: {
                if(_isHovered) {

                }
                break;
            }

            case sf::Event::MouseButtonReleased: {
                if(_isHovered) {
                    if(_callbackWithWidget)
                        return std::get< std::function<bool(Widget*)> >(_callback)(this);
                    else
                        return std::get< std::function<bool()> >(_callback)();
                }
                break;
            }

            case sf::Event::MouseMoved: {
                sf::Vector2u size = getSize();
                if(
                    event.mouseMove.x >= 0 && event.mouseMove.x >= 0 && 
                    event.mouseMove.x <= size.x && event.mouseMove.y <= size.y
                )
                    _isHovered = true;
                else
                    _isHovered = false;
                break;
            }

            default:
                break;
        }
    }

	void ButtonWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const{

    }

	void ButtonWidget::_update(float deltaTime) {
    }
}