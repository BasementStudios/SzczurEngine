#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include "SkillArea.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    class Widget; class ImageWidget; class WindowWidget;
    class GUI;

    class ProfessionBar : public BaseBar
    {
    public:
        ProfessionBar(SkillArea& skillArea);

        void setTexture(sf::Texture* texture);
        void setProfession(const std::string& profession);

        void initAssetsViaGUI(GUI& gui);

        const std::string& getProfession() const;
    private:
        SkillArea& _skillArea;

        ImageWidget* _icon{nullptr};
        WindowWidget* _window{nullptr};
        std::string _profession;

        void _onClick();
    };
}