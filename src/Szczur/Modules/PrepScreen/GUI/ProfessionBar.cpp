#include "ProfessionBar.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"

namespace rat
{
    ProfessionBar::ProfessionBar(SkillArea& skillArea)
    :
    _skillArea(skillArea)
    {
        _window = new WindowWidget;

        _icon = new ImageWidget;
        auto click = [&](Widget* owner){
            _onClick();
        };
        float dim = _dim - 2.f * _pad;
        _icon->setSize(dim, dim);
        _window->add(_icon);
        _window->setCallback(Widget::CallbackType::onPress, click);
    }

    void ProfessionBar::setPosition(float x, float y)
    {
        _window->setPosition(x, y);
    }

    void ProfessionBar::initAssetsViaGUI(GUI& gui)
    {
        _icon->setTexture(gui.getAsset<sf::Texture>("assets/PrepScreen/" + _profession + "Icon.png"));
        _window->setTexture(gui.getAsset<sf::Texture>("assets/Test/Window.png"), 200);
        _window->setScale(0.2f, 0.2f);
        _window->setPadding(_pad, _pad);
    }
    
    void ProfessionBar::setParent(Widget* parent)
    {
        parent->add(_window);
    }
    void ProfessionBar::setProfession(const std::string& profession)
    {
        _profession = profession;
    }
    const std::string& ProfessionBar::getProfession() const
    {
        return _profession;
    }

    void ProfessionBar::_onClick()
    {
        _skillArea.setProfession(_profession);
    }
    
}