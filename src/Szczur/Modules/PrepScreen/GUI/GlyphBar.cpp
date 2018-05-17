#include "GlyphBar.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"

#include "GrayPPArea.hpp"
#include "../ResourcesContainer.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    GlyphBar::GlyphBar(GrayPPArea& source)
    :
    _source(source)
    {
        _container = new ImageWidget;
        _glyph = new ImageWidget;
        _amountState = new TextWidget;

        _addWidget(_container);
        _container->setSize(100, 100);
        float padding = float(100 - 14) * _circleShiftProp;
        _container->setPadding(padding, padding);
        _glyph->setCallback(Widget::CallbackType::onPress, [&](Widget* owner){
            _onClick();
        });
        _container->add(_glyph);
        _glyph->setSize(sf::Vector2u{100 - 14, 100 - 14} - static_cast<sf::Vector2u>(_container->getPadding() * 2.f));


        _container->add(_amountState);
        _amountState->setPropOrigin(1.f, 1.f);
        _amountState->setPosition(sf::Vector2f{90, 90});
        _amountState->setCharacterSize(17);
        _amountState->setColor(sf::Color::White);
    }

    void GlyphBar::setType(GlyphID glyph)
    {
        _type = glyph;
        _updateText();
    }
    void GlyphBar::initAssetsViaGUI(GUI& gui)
    {
        const std::string path = "Assets/Test/";

        for(size_t i = 0; i < _glyphTextures.size(); i++)
        {
            _glyphTextures[i] = gui.getAsset<sf::Texture>(path + GlyphesConverter().toString(_type) /*+ to_string(i) + */+ "Glyph.png");
        }

        _container->setTexture(gui.getAsset<sf::Texture>(path + "GlyphCircle.png"));
        _glyph->setTexture(gui.getAsset<sf::Texture>(path + GlyphesConverter().toString(_type) + "Glyph.png"));
        _amountState->setFont(gui.getAsset<sf::Font>("Assets/fonts/NotoMono.ttf"));
    }

    void GlyphBar::_onClick()
    {
        auto& glyphContainer = _source.getSource().glyphContainer;
        auto glyphAmount = glyphContainer.getGlyphAmount(_type);
        auto glyphTotal = glyphContainer.getGlyphTotalAmount(_type);

        std::cout << "Type: " << GlyphesConverter().toString(_type) << " amount: " << glyphAmount << " total: " << glyphTotal << "\n";

        auto& ppContainer = _source.getSource().ppContainer;
                ppContainer.add();
                _source.recalculate();

        if(glyphAmount < glyphTotal)
        {
            if(glyphAmount == 0)
            {
                auto& ppContainer = _source.getSource().ppContainer;
                ppContainer.add();
                _source.recalculate();
            }
            glyphContainer.activeGlyph(_type);
            _updateText();
            _glyph->setTexture(_glyphTextures[glyphAmount + 1]);
        }
    }
    void GlyphBar::_updateText()
    {
        auto& glyphContainer = _source.getSource().glyphContainer;
        auto glyphAmount = glyphContainer.getGlyphAmount(_type);
        auto glyphTotal = glyphContainer.getGlyphTotalAmount(_type);
        _amountState->setString(std::to_string(glyphAmount) + "/" + std::to_string(glyphTotal));
    }

}