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
        _aboutToRecalculate = true;
    }

    void ImageWidget::setTexture(sf::Texture* texture) 
    {
        if(texture)
        {
            _sprite.setTexture(*texture);
            _hasTexture = true;
        }
        else
        {
            _hasTexture = false;
        }
        _aboutToRecalculate = true;
    }

    const sf::Texture* ImageWidget::getTexture() const 
    {
        if(_hasTexture) return _sprite.getTexture();
        return nullptr;
    }

    sf::Vector2u ImageWidget::_getSize() const 
    {
        if(_hasTexture) return {(unsigned int)_sprite.getGlobalBounds().width, (unsigned int)_sprite.getGlobalBounds().height};
        return {};
    }

    void ImageWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const 
    {
        if(_hasTexture) target.draw(_sprite, states);
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

    void ImageWidget::_setColor(const sf::Color& color)
    {
        _sprite.setColor(color);
    }
    
    
}