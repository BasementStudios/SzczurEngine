#include "ButtonWidget.hpp"

#include <iostream>

namespace rat {
    ButtonWidget::ButtonWidget(Widget* parent) :
        Widget(parent),
        _isHovered(false) {
        ;
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