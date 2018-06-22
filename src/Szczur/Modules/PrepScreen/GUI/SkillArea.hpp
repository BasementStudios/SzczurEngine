#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include "SkillBar.hpp"
#include "../Skill/Skill.hpp"
#include "../GlyphTypes.hpp"

#include <unordered_map>
#include <set>
#include <vector>
#include <memory>

#include "InfoBar.hpp"

namespace rat
{
    class PrepScreen;

    class SkillArea : public BaseBar
    {
        using SkillBars_t = std::vector<std::unique_ptr<SkillBar>>;
    public:
        SkillArea(PrepScreen& prepScreen);

        void initAssetsViaGUI(GUI& gui);

        void setSkills(const std::vector<const Skill*>& skills);
        void recalculateSkillsAvailability();

    private:
        SkillBars_t _skillBars;

        sf::Texture* _textureBar{nullptr};
        sf::Texture* _textureLocked{nullptr};
        sf::Font* _font{nullptr};

        WindowWidget* _border{nullptr};
        ScrollAreaWidget* _skillsScroller{nullptr};
        ListWidget* _skillsList{nullptr};

        InfoBar _infoBar;

        void _initNewSkillBarsAmount(size_t newAmount);
    };
}