#pragma once 

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

namespace rat
{
    class Enemy;
    class GUI;
    class EnemyArea;
    class EnemyBar : public BaseBar
    {
    public:
        EnemyBar(EnemyArea& parentArea);

        void setEnemy(const Enemy* enemy);
        void initAssetsViaGUI(GUI& gui);
    private:
        const Enemy* _enemy{nullptr};
        EnemyArea& _parentArea;

        WindowWidget* _border{nullptr};
        ImageWidget* _icon{nullptr};
        TextWidget* _name{nullptr};
        TextWidget* _hp{nullptr};

        WindowWidget* _filter{nullptr};

        Widget* _descParent{nullptr};
        TextAreaWidget* _description{nullptr};

        void _onHoverIn();
        void _onHoverOut();

    };
}