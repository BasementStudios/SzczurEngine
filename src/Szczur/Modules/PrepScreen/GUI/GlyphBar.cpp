#include "GlyphBar.hpp"

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/PrepScreen/PrepScreen.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"


#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    GlyphBar::GlyphBar(PrepScreen& prepScreen)
    :
    _prepScreen(prepScreen)
    {
        _container = new ImageWidget;
        _glyph = new ImageWidget;
        _amountState = new TextWidget;

        _addWidget(_container);
        _container->setPropSize(0.15f, 0.15f);
        _container->setCallback(Widget::CallbackType::onPress, [&](Widget* owner){
            _onClick();
        });
        _container->makeChildrenPenetrable();
        _container->add(_glyph);
        _glyph->setPropSize(0.13f, 0.13f);
        _glyph->setPropPosition(0.5f, 0.5f);
        _glyph->setStaticTexPositing();
        _glyph->setFullyTexSizing();
        //_glyph->setPosition(10.f, 10.f);


        _container->add(_amountState);
        _amountState->setPropPosition(13.f, 13.f);
        _amountState->setCharacterSize(17);
        _amountState->setColor(sf::Color::White);
    }

    void GlyphBar::setType(GlyphID glyph)
    {
        _type = glyph;
        setAmount(0, 0);
    }
    void GlyphBar::initAssetsViaGUI(GUI& gui)
    {
        const std::string path = "Assets/PrepScreen/";

        for(size_t i = 0; i < _glyphTextures.size(); i++)
        {
            _glyphTextures[i] = gui.getAsset<sf::Texture>(path + GlyphesConverter().toString(_type) /*+ to_string(i) + */+ "Glyph.png");
        }

        _container->setTexture(gui.getAsset<sf::Texture>("Assets/Test/GlyphCircle.png"));
        _glyph->setTexture(gui.getAsset<sf::Texture>(path + GlyphesConverter().toString(_type) + "Glyph.png"));
        _amountState->setFont(gui.getAsset<sf::Font>("Assets/fonts/NotoMono.ttf"));
    }

    void GlyphBar::_onClick()
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            if(_prepScreen.canBeGlyphDeactivated(_type))
            {
                _prepScreen.deactivateGlyph(_type);
            }
        }
        else
        {
            if(_prepScreen.canBeGlyphActivated(_type))
            {
                _prepScreen.activateGlyph(_type);
            }
        }
    }

    void GlyphBar::setAmount(size_t activated, size_t total)
    {
        _amountState->setString(std::to_string(activated) + "/" + std::to_string(total));

        float prop = total == 0 ? 0.f : float(activated) / float(total);

        _glyph->setPropTextureRect({ sf::Vector2f{0.f, 1.f - prop}, sf::Vector2f{1.f, prop} });
    }

}