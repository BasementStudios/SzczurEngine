#pragma once

#include "SkillArea.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    class Widget; class ImageWidget; class WindowWidget;
    class GUI;

    class ProfessionBar
    {
    public:
        ProfessionBar(SkillArea& skillArea);

        void setPosition(float x, float y);
        void setTexture(sf::Texture* texture);
        void setParent(Widget* parent);
        void setProfession(const std::string& profession);

        void initAssetsViaGUI(GUI& gui);

        const std::string& getProfession() const;
        constexpr static float _dim = 130.f; 
        constexpr static float _pad = 15.f; 

    private:
        SkillArea& _skillArea;

        ImageWidget* _icon{nullptr};
        WindowWidget* _window{nullptr};
        std::string _profession;

        void _onClick();
    };
}