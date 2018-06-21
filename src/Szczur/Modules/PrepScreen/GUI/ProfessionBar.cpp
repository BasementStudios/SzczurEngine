#include "ProfessionBar.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"

#include "Szczur/Utility/Logger.hpp"

#include <ctime>

namespace rat
{
    ProfessionBar::ProfessionBar(SkillArea& skillArea)
    :
    _skillArea(skillArea),
    BaseBar()
    {
        _window = new WindowWidget;
        _addWidget(_window);
        _window->makeChildrenPenetrable();
        _window->setCallback(Widget::CallbackType::onHoverIn, [](Widget* owner){
            owner->setColor({180, 180, 180}, 0.3f);
        });
        _window->setCallback(Widget::CallbackType::onHoverOut, [](Widget* owner){
            owner->setColor({255, 255, 255}, 0.3f);
        });

        _icon = new ImageWidget;

        auto click = [&](Widget* owner){
            _onClick();
        };
        _icon->setPropSize(0.17f, 0.17f);
        _icon->setPropPosition(0.5f, 0.5f);
        _window->add(_icon);
        _window->setCallback(Widget::CallbackType::onPress, click);
    }

    void ProfessionBar::initAssetsViaGUI(GUI& gui)
    {
        std::string path = "Assets/PrepScreen/" + _profession + "Icon.png";
        gui.addAsset<sf::Texture>(path);
        _icon->setTexture(gui.getAsset<sf::Texture>(path));
        _window->setTexture(gui.getAsset<sf::Texture>("Assets/Test/Window.png"), 200);
        _window->setScale(0.2f, 0.2f);
        _window->setPadding(15.f, 15.f);
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
        //clock_t t = clock();        
        _skillArea.setProfession(_profession);
        //float time = float(clock() - t);
        //std::cout << "Time elapsed: " << time/CLOCKS_PER_SEC << "s\n";
    }
    
}