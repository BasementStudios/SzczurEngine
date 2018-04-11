#pragma once

#include "SkillArea.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    class Widget; class ImageWidget;

    class ProfessionBar
    {
    public:
        ProfessionBar(SkillArea& skillArea);

        void setPosition(float x, float y);
        void setTexture(sf::Texture* texture);
        void setParent(Widget* parent);
        void setProfession(const std::string& profession);

        const std::string& getProfession() const;
        constexpr static float dim = 120.f; 

    private:
        SkillArea& _skillArea;

        ImageWidget* _icon{nullptr};
        std::string _profession;

        void _onClick();
    };
}