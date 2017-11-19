#include "CheckboxWidget.hpp"


namespace rat {
    CheckboxWidget::CheckboxWidget(Widget* parent) :
        Widget(parent),
        _isEnabled(false) {
        _textureOn.loadFromFile("data/check-box.png");
        _textureOff.loadFromFile("data/check-box-empty.png");
        _sprite.setTexture(_textureOn);

        _tempBackground.setFillColor(sf::Color(230,230,230));
        _tempBackground.setSize(  (sf::Vector2f)_textureOn.getSize() );
        
    }
    void CheckboxWidget::_input(const sf::Event& event) {
        if(event.type == sf::Event::MouseButtonReleased) {
            if(event.mouseButton.x <= _textureOn.getSize().x && event.mouseButton.y <= _textureOn.getSize().y){
                if(_isEnabled) {
                    _isEnabled = false;
                    _sprite.setTexture(_textureOff);
                }
                else if(! _isEnabled) {
                    _isEnabled = true;
                    _sprite.setTexture(_textureOn);
                }
            }
        }
    }

    void CheckboxWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(_tempBackground, states);
        target.draw(_sprite, states);
    }

	void CheckboxWidget::_update(float deltaTime) {

    }
}