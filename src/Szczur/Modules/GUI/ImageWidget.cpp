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

    void ImageWidget::setScale(const sf::Vector2f& scale) {
        _sprite.setScale(scale);
        calculateSize();
    }

    void ImageWidget::setTexture(sf::Texture* texture) {
        _sprite.setTexture(*texture);
        _hasTexture = true;
        calculateSize();
    }

    const sf::Texture* ImageWidget::getTexture() const {
        return _sprite.getTexture();
    }

    sf::Vector2u ImageWidget::_getSize() const {
        return {(size_t)_sprite.getGlobalBounds().width, (size_t)_sprite.getGlobalBounds().height};
    }

    void ImageWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if(_isActivated)
            target.draw(_sprite, states);
    }

    void ImageWidget::_callback(CallbackType type) {
        if(auto it = _luaCallbacks.find(type); it != _luaCallbacks.end())
            std::invoke(it->second, this);
        if(auto it = _callbacks.find(type); it != _callbacks.end())
            std::invoke(it->second, this);
    }

    void ImageWidget::_calculateSize()
    {
        if(!_hasTexture) return;

        auto texSize = static_cast<sf::Vector2f>(_sprite.getTexture()->getSize());
        sf::Vector2f scale = {1.f, 1.f};
        if(_isMinSizeSet)
        {
            auto minSize = static_cast<sf::Vector2f>(getMinimalSize());
            scale = {minSize.x / texSize.x, minSize.y / texSize.y};            
        }
        _sprite.setScale(scale);
    }
    
}