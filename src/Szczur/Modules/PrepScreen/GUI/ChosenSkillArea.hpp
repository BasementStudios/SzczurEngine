#pragma once

#include <vector>
#include <memory>

#include "ChosenSkillBar.hpp"

namespace rat
{
    class Widget;
    class SkillArea;

    class ChosenSkillArea
    {
    public:
        ChosenSkillArea(SkillArea& parentArea, size_t size = 6);
        void setPosition(float x, float y);
        void setParent(Widget* parent);
        void initTexturesViaGUI(GUI& gui);
        void addSkill(Skill* skill);
        bool hasFreeSpace() const;
        void recalculate();
        SkillArea& getSkillArea() const;
    private:
        SkillArea& _parentArea;
        size_t _size;

        Widget* _base{nullptr};

        std::vector<std::unique_ptr<ChosenSkillBar>> _skillBars;
    };
}