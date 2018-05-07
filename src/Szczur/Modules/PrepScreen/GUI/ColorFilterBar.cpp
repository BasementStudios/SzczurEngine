#include "ColorFilterBar.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

#include "ColorFilterArea.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    ColorFilterBar::ColorFilterBar(ColorFilterArea& parentArea)
    :
    BaseBar(),
    _parentArea(parentArea)
    {
        _backIcon = new ImageWidget;
        _addWidget(_backIcon);
        _backIcon->setPosition(-_backPadding, -_backPadding);
        _backIcon->invisible();

        _icon = new ImageWidget;
        _addWidget(_icon);
        auto onClick = [&](Widget* owner){
            _onClick();
        };
        _icon->setCallback(Widget::CallbackType::onPress, onClick);
    }

    void ColorFilterBar::initAssetsViaGUI(GUI& gui)
    {
        auto* iconTexture = gui.getAsset<sf::Texture>("Assets/PrepScreen/" + _color + "PP.png");
        _icon->setTexture(iconTexture);
        _setWidgetSize(_icon, _dim, _dim);
        auto* backTexture = gui.getAsset<sf::Texture>("Assets/PrepScreen/PP.png");
        _backIcon->setTexture(backTexture);
        _setWidgetSize(_backIcon, _dim + 2.f * _backPadding, _dim +2.f * _backPadding);
    }

    void ColorFilterBar::setColor(const std::string& color)
    {
        _color = color;
    }
    
    void ColorFilterBar::activate()
    {
        _icon->activate();
        _icon->visible();
        if(_isChosen) _backIcon->visible();
    }
    void ColorFilterBar::deactivate()
    {
        _icon->deactivate();
        _icon->invisible();
        _backIcon->invisible();
    }

    float ColorFilterBar::getDimension() const
    {
        return _dim;
    }
    void ColorFilterBar::unchoose()
    {
        _isChosen = false;
        _backIcon->invisible();
    }
    
    

    void ColorFilterBar::_onClick()
    {
        bool adding = true; 
        
        if(adding)
        {
            if(_isChosen)
            {
                _backIcon->invisible();
                _parentArea.removeColor(_color);
                _isChosen = false;
            }
            else
            {
                if(_parentArea.canBeAdded(_color))
                {
                    _backIcon->visible();
                    _parentArea.addColor(_color);
                    _isChosen = true;                    
                }
            }
        }
        else
        {
            if(!_isChosen)
            {
                _parentArea.unchooseAll();
                _isChosen = true;
                _parentArea.setColor(_color);
                _backIcon->visible();
            }
        }
    }
    
}