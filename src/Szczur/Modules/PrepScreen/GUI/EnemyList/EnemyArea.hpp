#pragma once 

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include <vector>
#include <memory>

#include "EnemyBar.hpp"

namespace sf { class Texture; class Font; }

namespace rat
{
    class EnemyCodex; class Enemy;
    class EnemyArea : public BaseBar
    {
    public:
        EnemyArea();

        void initAssetsViaGUI(GUI& gui);
        void initEnemiesViaCodex(EnemyCodex& codex);

        void clear();
        void pushEnemy(const Enemy* enemy, GUI& gui);

        void setEnemyInfo(Enemy* enemy, const sf::Vector2f& pos = {});
        bool isEnemyInInfo(Enemy* enemy);
        void deactivateInfo();
    private:
        std::vector<std::unique_ptr<EnemyBar>> _enemyBars;

        WindowWidget* _border{nullptr};

        ScrollAreaWidget* _scroller{nullptr};
        ListWidget* _list{nullptr};

        Enemy* _chosenEnemy{nullptr};


    };
}