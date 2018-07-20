#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include <vector>
#include <memory>

#include "ChosenSkillBar.hpp"

namespace rat
{
    class PrepScreen;

    class ChosenSkillArea : public BaseBar
    {
    public:
        ChosenSkillArea(PrepScreen& prepScreen, size_t size = 6);
        void initAssetsViaGUI(GUI& gui);
        void addSkill(const Skill* skill);
        bool hasFreeSpace() const;
        void recalculate();
    private:
        size_t _size;
        size_t _ocuppied{0};

        sf::Vector2f _iconSize;
        ListWidget* _skillsList{nullptr};
        ImageWidget* _border{nullptr};

        std::vector<std::unique_ptr<ChosenSkillBar>> _skillBars;
    };
}