#include "EnemyBar.hpp"
#include "EnemyArea.hpp"

#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

#include "../../Enemy/Enemy.hpp"


namespace rat
{
    EnemyBar::EnemyBar(EnemyArea& parentArea)
    :
    _parentArea(parentArea)
    {
        sf::Vector2f size = {240.f, 72.f};
        setSize(size);

        _border = new WindowWidget;
        _icon = new ImageWidget;
        _name = new TextWidget;
        _hp = new TextWidget;

        _addWidget(_border);
        _border->setScale(0.2f, 0.2f);
        _border->setPadding(5.f, 2.f);

        _border->setCallback(Widget::CallbackType::onHoverIn, [&](Widget*){
            _onHoverIn();
        });
        _border->setCallback(Widget::CallbackType::onHoverOut, [&](Widget*){
            _onHoverOut();
        });

        _border->add(_icon);
        unsigned int dim = size.y - 4;
        _icon->setSize(dim, dim);

        _border->add(_name);
        _name->setCharacterSize(20);
        _name->setPropOrigin(1.f, 0.f);
        _name->setPosition(size.x - 4, 0);

        _border->add(_hp);
        _hp->setCharacterSize(20);
        _hp->setPropOrigin(1.f, 0.f);
        _hp->setPosition(size.x - 4, 30);
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
        _parentArea.setEnemyInfo(_enemy, {0.f, getPosition().y});
    }
    void EnemyBar::_onHoverOut()
    {
        if(!_enemy) return;
        if(_parentArea.isEnemyInInfo(_enemy) || true) _parentArea.deactivateInfo();
    }
}