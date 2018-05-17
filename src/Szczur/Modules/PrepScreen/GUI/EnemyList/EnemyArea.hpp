#pragma once 

#include "../BaseBar.hpp"

#include <vector>
#include <memory>

#include "EnemyBar.hpp"
#include "../InfoBar.hpp"

namespace rat
{
    class ScrollAreaWidget;
    class GUI; class EnemyCodex; class Enemy;
    class EnemyArea : public BaseBar
    {
    public:
        EnemyArea();

        void initAssetsViaGUI(GUI& gui);
        void initEnemiesViaCodex(EnemyCodex& codex);

        void setEnemyInfo(Enemy* enemy, const sf::Vector2f& pos = {});
        bool isEnemyInInfo(Enemy* enemy);
        void deactivateInfo();
    private:
        std::vector<std::unique_ptr<EnemyBar>> _enemyBars;

        WindowWidget* _border{nullptr};
        ScrollAreaWidget* _scroller{nullptr};

        InfoBar _infoBar;
        Enemy* _chosenEnemy{nullptr};

    };
}