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
        _container->setPropSize(0.125f, 0.125f);
        _container->setCallback(Widget::CallbackType::onPress, [&](Widget* owner){
            _onClick();
        });
        _container->makeChildrenPenetrable();
        _container->add(_glyph);
        _glyph->setPropSize(0.08f, 0.08f);
        _glyph->setPropPosition(0.5f, 0.5f);


        _container->add(_amountState);
        _amountState->setPropPosition(1.f, 1.f);
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
                //_source.recalculate();

        if(glyphAmount < glyphTotal)
        {
            if(glyphAmount == 0)
            {
                auto& ppContainer = _source.getSource().ppContainer;
                ppContainer.add();
                //_source.recalculate();
            }
            glyphContainer.activateGlyph(_type);
            _updateText();
            _glyph->setTexture(_glyphTextures[glyphAmount + 1]);

            //prepScreen.activateGlyph(_type);
        }
    }
    void GlyphBar::_updateText()
    {
        auto& glyphContainer = _source.getSource().glyphContainer;
        auto glyphAmount = glyphContainer.getGlyphAmount(_type);
        auto glyphTotal = glyphContainer.getGlyphTotalAmount(_type);
        _amountState->setString(std::to_string(glyphAmount) + "/" + std::to_string(glyphTotal));
    }

    void GlyphBar::setAmount(size_t activated, size_t total)
    {
        _amountState->setString(std::to_string(activated) + "/" + std::to_string(total));
    }

}