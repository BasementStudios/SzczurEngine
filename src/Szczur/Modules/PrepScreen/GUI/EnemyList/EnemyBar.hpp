#pragma once 

#include "../BaseBar.hpp"

namespace rat
{
    class Enemy;
    class GUI;
    class EnemyArea;
    class EnemyBar : public BaseBar
    {
    public:
        EnemyBar(EnemyArea& parentArea);

        void setEnemy(Enemy* enemy);
        void initAssetsViaGUI(GUI& gui);
    private:
        Enemy* _enemy{nullptr};
        EnemyArea& _parentArea;

        WindowWidget* _border{nullptr};
        ImageWidget* _icon{nullptr};
        TextWidget* _name{nullptr};
        TextWidget* _hp{nullptr};

        void _onHoverIn();
        void _onHoverOut();

    };
}