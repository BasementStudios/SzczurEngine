#include "EnemyBar.hpp"

#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

#include "../../Enemy/Enemy.hpp"


namespace rat
{
    EnemyBar::EnemyBar()
    {
        sf::Vector2u size = {240, 72};
        setSize(size);

        _border = new WindowWidget;
        _icon = new ImageWidget;
        _name = new TextWidget;
        _hp = new TextWidget;

        _addWidget(_border);
        _border->setScale(0.2f, 0.2f);
        _border->setPadding(2.f, 2.f);

        _border->add(_icon);
        _icon->setSize(size - sf::Vector2u{4, 4});

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
    }

    void EnemyBar::initAssetsViaGUI(GUI& gui)
    {
        auto* barTex = gui.getAsset<sf::Texture>("Assets/Test/Bar.png");
        _border->setTexture(barTex, 6);
        _name->setFont(gui.getAsset<sf::Font>("Assets/fonts/NotoMono.ttf"));
        _hp->setFont(gui.getAsset<sf::Font>("Assets/fonts/NotoMono.ttf"));
    }
}