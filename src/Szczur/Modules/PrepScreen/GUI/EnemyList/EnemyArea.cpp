#include "EnemyArea.hpp"

#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

#include "../../Enemy/EnemyCodex.hpp"
#include "../../Enemy/Enemy.hpp"

namespace rat
{
    EnemyArea::EnemyArea()
    {
        //_getBase()->setPropPosition(0.f, 0.5f);
        _border = new WindowWidget;
        _addWidget(_border);
        //_border->setScale(0.2f, 0.2f);
        _border->setMainPatchPropSize({0.3f, 0.3f});
        _border->setPropSize(0.463f, 0.85f);

        _scroller = new ScrollAreaWidget;
        _scroller->setPropSize(0.37f, 0.75f); //0.65f
        _scroller->setPropPosition(0.5f, 0.5f);
        _scroller->makeScrollAutoHiding();
        _border->add(_scroller);

        _list = new ListWidget;
        _scroller->add(_list);
        _list->setPropPosition(0.2f, 0.f);
        _list->setPropBetweenPad(0.01f);
    }
        
    void EnemyArea::initAssetsViaGUI(GUI& gui)
    {

        _scroller->setPathTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBar.png"));
        _scroller->setScrollerTexture(gui.getAsset<sf::Texture>("Assets/Test/Scroller.png"));
        _scroller->setBoundsTexture(gui.getAsset<sf::Texture>("Assets/Test/ScrollerBound.png"));

        _border->setTexture(gui.getAsset<sf::Texture>("Assets/PrepScreen/SkillBack.png"), 30);

        for(auto& enemyBar : _enemyBars)
        {
            enemyBar->initAssetsViaGUI(gui);
        }
    }
    void EnemyArea::initEnemiesViaCodex(EnemyCodex& codex)
    {
        for(auto& [name, enemy] : codex)
        {
            auto bar = std::make_unique<EnemyBar>(*this);
            bar->setEnemy(enemy.get());
            bar->setParent(_list);
            _enemyBars.emplace_back(std::move(bar));
        }
    }
    void EnemyArea::setEnemyInfo(Enemy* enemy, const sf::Vector2f& pos)
    {
    }
    bool EnemyArea::isEnemyInInfo(Enemy* enemy)
    {
        return _chosenEnemy == enemy;
    }
    void EnemyArea::deactivateInfo()
    {
        _chosenEnemy = nullptr;
    }
}