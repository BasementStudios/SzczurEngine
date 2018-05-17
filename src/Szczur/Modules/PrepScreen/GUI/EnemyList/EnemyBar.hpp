#pragma once 

#include "../BaseBar.hpp"

namespace rat
{
    class Enemy;
    class GUI;
    class EnemyBar : public BaseBar
    {
    public:
        EnemyBar();

        void setEnemy(Enemy* enemy);
        void initAssetsViaGUI(GUI& gui);
    private:
        Enemy* _enemy{nullptr};

        WindowWidget* _border{nullptr};
        ImageWidget* _icon{nullptr};
        TextWidget* _name{nullptr};
        TextWidget* _hp{nullptr};
    };
}