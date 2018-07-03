#include "ImageWidget.hpp"

#include <string>
#include <iostream>
#include <cassert>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "Animation/Anim.hpp"

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
            if(_hasPropTexRect) _calcPropTexRect();
        }
        else
        {
            LOG_ERROR("Texture given to ImageWidget is nullptr");
            _hasTexture = false;
        }
        _aboutToRecalculate = true;
    }

    void ImageWidget::removeTexture()
    {
        _hasTexture = false;
        _aboutToRecalculate = true;
    }

    void ImageWidget::setTextureRect(const sf::IntRect& rect)
    {
        _hasPropTexRect = false;
        _sprite.setTextureRect(rect);
        _aboutToRecalculate = true;
        if(_isStaticTexPositing && _isFullyTexSizing) _calcStaticSizing();
    }
    void ImageWidget::setPropTextureRect(const sf::FloatRect& propRect)
    {
        _hasPropTexRect = true;
        _propTexRect = propRect;
        _calcPropTexRect();
        if(_isStaticTexPositing && _isFullyTexSizing) _calcStaticSizing();
    }

    void ImageWidget::setPropTextureRectInTime(const sf::FloatRect& propRect, const gui::AnimData& data)
    {
        using TexRectAnim_t = gui::Anim<ImageWidget, gui::AnimType::TexRect, sf::FloatRect>;
        auto setter = static_cast<void (ImageWidget::*)(const sf::FloatRect&)>(&ImageWidget::setPropTextureRect);

        auto anim = std::make_unique<TexRectAnim_t>(this, setter);
        anim -> setAnim(_propTexRect, propRect, data);
        _addAnimation(std::move(anim));
    }
    void ImageWidget::setPropTextureRectInTime(const sf::FloatRect& propRect, float time)
    {
        setPropTextureRectInTime(propRect, gui::AnimData(time));
    }

    void ImageWidget::setFullyTexSizing()
    {
        _isFullyTexSizing = true;
        if(_hasPropTexRect) _aboutToRecalculate = true;
    }

    void ImageWidget::setStaticTexPositing()
    {
        _isStaticTexPositing = true;
        if(_hasPropTexRect && _isFullyTexSizing) _calcStaticSizing();
    }
    void ImageWidget::_calcStaticSizing()
    {
        assert(_isStaticTexPositing && _isFullyTexSizing);
        auto texRect = _sprite.getTextureRect();
        _sprite.setOrigin({-float(texRect.left), -float(texRect.top)});
    }

    void ImageWidget::_calcPropTexRect()
    {
        assert(_hasPropTexRect);
        if(!_hasTexture) return;

        auto texSize = static_cast<sf::Vector2f>(_sprite.getTexture()->getSize());

        sf::Vector2i pos( int(_propTexRect.left * texSize.x), int(_propTexRect.top * texSize.y) );
        sf::Vector2i size( int(_propTexRect.width * texSize.x), int(_propTexRect.height * texSize.y) );

        sf::IntRect texRect = { pos, size };

        _aboutToRecalculate = true;

        _sprite.setTextureRect(texRect);
    }
    const sf::Texture* ImageWidget::getTexture() const  
    { 
        if(_hasTexture) return _sprite.getTexture(); 
        return nullptr; 
    }

    sf::Vector2f ImageWidget::_getSize() const 
    {
        if(_hasTexture) return {(float)_sprite.getGlobalBounds().width, (float)_sprite.getGlobalBounds().height};
        return {};
    }

    void ImageWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const 
    {
        if(_hasTexture) target.draw(_sprite, states);
    }

    void ImageWidget::_calculateSize()
    {
        if(!_hasTexture) return;

        sf::Vector2f texSize;
        if(_isFullyTexSizing)
        {
            texSize = static_cast<sf::Vector2f>(_sprite.getTexture()->getSize());
        }
        else
        {
            texSize = sf::Vector2f{float(_sprite.getTextureRect().width), float(_sprite.getTextureRect().height)};
        }
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

    void ImageWidget::_recalcPos()
    {
        _sprite.setPosition(static_cast<sf::Vector2f>(gui::FamilyTransform::getDrawPosition()));
    }

    
    
    
}