#include "CheckboxWidget.hpp"
#include <iostream>
namespace rat {
    CheckboxWidget::CheckboxWidget() :
        _isEnabled(false),
        isHover(false) {
        _textureOn.loadFromFile("data/check-box.png");
        _textureOff.loadFromFile("data/check-box-empty.png");
        _sprite.setTexture(_textureOff);

        _tempBackground.setFillColor(sf::Color(230,230,230));
        _tempBackground.setSize(  (sf::Vector2f)_textureOn.getSize() );
        
    }

    void CheckboxWidget::onPress() {
        if(_isEnabled) {
            _isEnabled = false;
            _sprite.setTexture(_textureOff);
        }
        else if(! _isEnabled) {
            _isEnabled = true;
            _sprite.setTexture(_textureOn);
        }
    }

    void CheckboxWidget::onRelease() {

    }

    bool CheckboxWidget::_input(const sf::Event& event) {
        switch(event.type) {
            case sf::Event::MouseButtonPressed: {
                if(event.mouseButton.x >= 0 && event.mouseButton.y >= 0)
                    if(event.mouseButton.x <= _textureOn.getSize().x && event.mouseButton.y <= _textureOn.getSize().y) {
                        onPress();
                    }
                break;
            }

            case sf::Event::MouseButtonReleased: {
                if(event.mouseButton.x >= 0 && event.mouseButton.y >= 0)
                    if(event.mouseButton.x <= _textureOn.getSize().x && event.mouseButton.y <= _textureOn.getSize().y) {
                        onRelease();
                    }
                break;
            }

            case sf::Event::MouseMoved: {
                if(event.mouseButton.x >= 0 && event.mouseButton.y >= 0){
                    if(event.mouseMove.x <= _textureOn.getSize().x && event.mouseMove.y <= _textureOn.getSize().y) {
                        isHover = true;
                    }
                    else
                        isHover = false;
                }
                else
                    isHover = false;
                break;
            }

            default:
                break;
        }
    }

    void CheckboxWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(_tempBackground, states);
        target.draw(_sprite, states);
    }

	void CheckboxWidget::_update(float deltaTime) {
        if(isHover){
            _tempBackground.setFillColor(sf::Color(230,0,230));
        }
        else
            _tempBackground.setFillColor(sf::Color(230,230,230));
    }
}