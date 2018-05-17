#include "EnemyArea.hpp"

#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

#include "../../Enemy/EnemyCodex.hpp"

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
    }
        
    void EnemyArea::initAssetsViaGUI(GUI& gui)
    {

        _scroller->setPathTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"));
        _scroller->setScrollerTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));
        _scroller->setBoundsTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));

        _border->setTexture(gui.getAsset<sf::Texture>("Assets/Test/Window.png"), 200);

        for(auto& enemyBar : _enemyBars)
        {
            enemyBar->initAssetsViaGUI(gui);
        }
    }
    void EnemyArea::initEnemiesViaCodex(EnemyCodex& codex)
    {
        for(auto& [name, enemy] : codex)
        {
            auto bar = std::make_unique<EnemyBar>();
            bar->setEnemy(enemy.get());
            _addBar(bar);
            _enemyBars.emplace_back(std::move(bar));
        }
    }
}