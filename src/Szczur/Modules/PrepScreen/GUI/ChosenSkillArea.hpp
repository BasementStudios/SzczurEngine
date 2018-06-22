#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include <vector>
#include <memory>

#include "ChosenSkillBar.hpp"

namespace rat
{
    class WindowWidget; class ListWidget;
    class SkillArea;

    class ChosenSkillArea : public BaseBar
    {
    public:
        ChosenSkillArea(size_t size = 6);
        void initAssetsViaGUI(GUI& gui);
        void addSkill(Skill* skill);
        bool hasFreeSpace() const;
        void recalculate();
    private:
        size_t _size;

        float _widthPadding = 70.f;
        sf::Vector2f _iconSize;
        float _scale{0.2f};
        WindowWidget* _border{nullptr};
        ListWidget* _skillsList{nullptr};

        std::vector<std::unique_ptr<ChosenSkillBar>> _skillBars;
    };
}