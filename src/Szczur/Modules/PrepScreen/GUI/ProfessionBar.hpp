#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include "SkillArea.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    class PrepScreen;

    class ProfessionBar : public BaseBar
    {
    public:
        ProfessionBar(PrepScreen& prepScreen);

        void setTexture(sf::Texture* texture);
        void setProfession(const std::string& profession);

        void initAssetsViaGUI(GUI& gui);

        const std::string& getProfession() const;
    private:
        PrepScreen& _prepScreen;
        ImageWidget* _icon{nullptr};
        WindowWidget* _window{nullptr};
        std::string _profession;

        void _onClick();
    };
}