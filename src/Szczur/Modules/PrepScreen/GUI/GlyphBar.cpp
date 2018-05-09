#include "GlyphBar.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    GlyphBar::GlyphBar()
    {
        _container = new ImageWidget;
        _glyph = new ImageWidget;
        _amountState = new TextWidget;

        _addWidget(_container);
        _container->setSize(100, 100);
        float padding = float(100 - 14) * _circleShiftProp;
        _container->setPadding(padding, padding);
        _container->setCallback(Widget::CallbackType::onRelease, [&](Widget* owner){
            _onClick();
        });
        std::cout << "Padding size: x: " << _container->getPadding().x << " y: " <<  _container->getPadding().y << "\n";
        _container->add(_glyph);
        _glyph->setSize(sf::Vector2u{100 - 14, 100 - 14} - static_cast<sf::Vector2u>(_container->getPadding() * 2.f));
        std::cout << "Glyph size: x: " << _glyph->getSize().x << " y: " << _glyph->getSize().y << "\n";


        _container->add(_amountState);
        _amountState->setPropOrigin(1.f, 1.f);
        _amountState->setPosition(sf::Vector2f{90, 90});
        _amountState->setCharacterSize(17);
        _amountState->setColor(sf::Color::White);
    }

    GlyphBar::GlyphBar(const std::string& type)
    :
    GlyphBar()
    {
        setType(type);
    }
    void GlyphBar::setType(const std::string& type)
    {
        _type = type;
    }

    void GlyphBar::setStartAmount(int amount)
    {
        _amount = std::min(amount, int(_glyphTextures.size()));
        _updateText();
    }
    //int GlyphBar::getUsedAmount() const;
    void GlyphBar::initAssetsViaGUI(GUI& gui)
    {
        const std::string path = "Assets/Test/";

        for(size_t i = 0; i < _glyphTextures.size(); i++)
        {
            _glyphTextures[i] = gui.getAsset<sf::Texture>(path + _type /*+ to_string(i) + */+ "Glyph.png");
        }

        _container->setTexture(gui.getAsset<sf::Texture>(path + "GlyphCircle.png"));
        _glyph->setTexture(gui.getAsset<sf::Texture>(path + _type + "Glyph.png"));
        _amountState->setFont(gui.getAsset<sf::Font>("Assets/fonts/NotoMono.ttf"));
    }

    void GlyphBar::_onClick()
    {
        if(_takenAmount < _amount)
        {
            _takenAmount++;
            _updateText();
            _glyph->setTexture(_glyphTextures[_takenAmount]);
        }
    }
    void GlyphBar::_updateText()
    {
        _amountState->setString(std::to_string(_takenAmount) + "/" + std::to_string(_amount));
    }

}