#include "EnemyBar.hpp"
#include "EnemyArea.hpp"

#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

#include "../../Enemy/Enemy.hpp"


namespace rat
{
    EnemyBar::EnemyBar(EnemyArea& parentArea)
    :
    _parentArea(parentArea)
    {
        const sf::Vector2f prSize(0.312037037037037f, 0.1148148148148f);

        _border = new WindowWidget;
        _icon = new ImageWidget;
        _name = new TextWidget;
        _hp = new TextWidget;

        _addWidget(_border);
        _border->setMainPatchPropSize({prSize.y, prSize.y});
        _border->setPropSize(prSize);
        _border->makeChildrenPenetrable();
        _border->makeChildrenUnresizable();

        _border->setCallback(Widget::CallbackType::onHoverIn, [this](Widget*){
            _onHoverIn();
        });
        _border->setCallback(Widget::CallbackType::onHoverOut, [this](Widget*){
            _onHoverOut();
        });

        auto* infoList = new ListWidget;
        _border->add(infoList);
        infoList->makeHorizontal();

        const sf::Vector2f prPad = {0.01f, 0.01f};

        auto* iconParent = new Widget;
        iconParent->setPropSize(prSize.y, prSize.y);
        //iconParent->setPropPadding(prPad); 
        infoList->add(iconParent);

        _icon->setPropSize(prSize.y - prPad.x, prSize.y - prPad.y);
        _icon->setPropPosition(0.5f, 0.5f);
        iconParent->add(_icon);

        auto* titles = new Widget;
        titles->setPropSize(prSize.x - prSize.y, prSize.y);
        titles->setPropPadding(0.01f, 0.01f);
        infoList->add(titles);

        titles->add(_name);
        _name->setCharacterSize(20);
        _name->setPropPosition(1.f, 0.f);

        titles->add(_hp);
        _hp->setCharacterSize(20);
        _hp->setPropPosition(1.f, 1.f);
    }

    void EnemyBar::setEnemy(Enemy* enemy)
    {
        _icon->setTexture(enemy->getTexture());
        _hp->setString("HP: " + std::to_string(enemy->getMaxHP()));
        _name->setString(enemy->getName());
        _enemy = enemy;
    }

    void EnemyBar::initAssetsViaGUI(GUI& gui)
    {
        auto* barTex = gui.getAsset<sf::Texture>("Assets/Test/Bar.png");
        _border->setTexture(barTex, 6);
        _name->setFont(gui.getAsset<sf::Font>("Assets/fonts/NotoMono.ttf"));
        _hp->setFont(gui.getAsset<sf::Font>("Assets/fonts/NotoMono.ttf"));
    }

    void EnemyBar::_onHoverIn()
    {
        if(!_enemy) return;    
        //_parentArea.setEnemyInfo(_enemy, {0.f, getPosition().y});
    }
    void EnemyBar::_onHoverOut()
    {
        if(!_enemy) return;
        //if(_parentArea.isEnemyInInfo(_enemy) || true) _parentArea.deactivateInfo();
    }
}