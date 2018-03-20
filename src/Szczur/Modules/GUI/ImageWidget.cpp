#include "ImageWidget.hpp"

#include <string>
#include <iostream>
#include "Test.hpp"
#include "Szczur/Modules/Script/Script.hpp"

namespace rat {
    ImageWidget::ImageWidget() :
    Widget() {

    }

    ImageWidget::ImageWidget(sf::Texture* texture) :
    Widget() {
        _sprite.setTexture(*texture);
    }

    void ImageWidget::initScript(Script& script) {
        auto object = script.newClass<ImageWidget>("ImageWidget", "GUI");
        //auto object = script.newClass<ImageWidget>("ImageWidget", "GUI");
        //Widget::basicScript<ImageWidget>(object);
        basicScript(object);

        object.setProperty(
            "texture",
            [](ImageWidget& owner){owner.getTexture();},
            [](ImageWidget& owner, sf::Texture* texture){owner.setTexture(texture);}
        );
        
        object.init();
    }

    void ImageWidget::setTexture(sf::Texture* texture) {
        _sprite.setTexture(*texture);
    }

    const sf::Texture* ImageWidget::getTexture() const {
        return _sprite.getTexture();
    }

    sf::Vector2u ImageWidget::_getSize() const {
        return _sprite.getTexture()->getSize();
    }

    void ImageWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(_sprite, states);
    }

    void ImageWidget::_callback(CallbackType type) {
        if(auto it = _luaCallbacks.find(type); it != _luaCallbacks.end())
            std::invoke(it->second, this);
        if(auto it = _callbacks.find(type); it != _callbacks.end())
            std::invoke(it->second, this);
    }
}