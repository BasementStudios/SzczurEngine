#pragma once

#include "BaseBar.hpp"

#include <vector>
#include <memory>

#include "ChosenSkillBar.hpp"

namespace rat
{
    class WindowWidget;
    class SkillArea;

    class ChosenSkillArea : public BaseBar
    {
    public:
        ChosenSkillArea(SkillArea& parentArea, size_t size = 6);
        void initAssetsViaGUI(GUI& gui);
        void addSkill(Skill* skill);
        bool hasFreeSpace() const;
        void recalculate();
        SkillArea& getSkillArea() const;
    private:
        SkillArea& _parentArea;
        size_t _size;



        float _widthPadding = 70.f;
        sf::Vector2f _iconSize;
        float _scale{0.2f};
        WindowWidget* _border{nullptr};
        Widget* _skills{nullptr};

        std::vector<std::unique_ptr<ChosenSkillBar>> _skillBars;
    };
}