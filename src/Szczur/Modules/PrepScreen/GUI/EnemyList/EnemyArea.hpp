#pragma once 

#include "../BaseBar.hpp"

#include <vector>
#include <memory>

#include "EnemyBar.hpp"

namespace rat
{
    class ScrollAreaWidget;
    class GUI; class EnemyCodex;
    class EnemyArea : public BaseBar
    {
    public:
        EnemyArea();

        void initAssetsViaGUI(GUI& gui);
        void initEnemiesViaCodex(EnemyCodex& codex);
    private:
        std::vector<std::unique_ptr<EnemyBar>> _enemyBars;

        WindowWidget* _border{nullptr};
        ScrollAreaWidget* _scroller{nullptr};
    };
}