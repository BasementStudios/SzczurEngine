#include "EnemyArea.hpp"

#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

#include "../../Enemy/EnemyCodex.hpp"
#include "../../Enemy/Enemy.hpp"

namespace rat
{
    EnemyArea::EnemyArea()
    {
        _border = new WindowWidget;
        _addWidget(_border);
        _border->setScale(0.2f, 0.2f);
        _border->setPadding(11.f, 11.f);

        _scroller = new ScrollAreaWidget;
        _scroller->setSize(300.f, 400.f);
        _border->add(_scroller);

        _addBar(_infoBar);
        _infoBar.deactivate();
        _infoBar.setPropOrigin(1.f, 0.f);
    }
        
    void EnemyArea::initAssetsViaGUI(GUI& gui)
    {

        _scroller->setPathTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"));
        _scroller->setScrollerTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));
        _scroller->setBoundsTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));

        _border->setTexture(gui.getAsset<sf::Texture>("Assets/Test/Window.png"), 200);

        _infoBar.initAssetsViaGUI(gui);

        for(auto& enemyBar : _enemyBars)
        {
            enemyBar->initAssetsViaGUI(gui);
        }
    }
    void EnemyArea::initEnemiesViaCodex(EnemyCodex& codex)
    {
        size_t i = 0;
        for(auto& [name, enemy] : codex)
        {
            auto bar = std::make_unique<EnemyBar>(*this);
            const auto height = bar->getSize().y + 10;
            bar->setPosition(0.f, float(height * i++));
            bar->setEnemy(enemy.get());
            bar->setParent(_scroller);
            _enemyBars.emplace_back(std::move(bar));
        }
    }
    void EnemyArea::setEnemyInfo(Enemy* enemy, const sf::Vector2f& pos)
    {
        _infoBar.setPosition(pos);
        _infoBar.setName(enemy->getName());
        _infoBar.activate();
    }
    bool EnemyArea::isEnemyInInfo(Enemy* enemy)
    {
        _chosenEnemy = enemy;
    }
    void EnemyArea::deactivateInfo()
    {
        _chosenEnemy = nullptr;
        _infoBar.deactivate();
    }
}