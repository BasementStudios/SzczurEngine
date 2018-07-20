#pragma once

#include "Szczur/Modules/GUI/Base/BaseBar.hpp"

#include "SkillBar.hpp"
#include "../SkillSlot.hpp"
#include "Szczur/Modules/PrepScreen/Skill/Skill.hpp"
#include "Szczur/Modules/PrepScreen/GlyphTypes.hpp"

#include <unordered_map>
#include <set>
#include <vector>
#include <memory>

namespace rat
{
    class PrepScreen;

    class SkillArea : public BaseBar
    {
        using SkillBars_t = std::vector<std::unique_ptr<SkillSlot>>;
    public:
        SkillArea(PrepScreen& prepScreen);

        void initAssetsViaGUI(GUI& gui);

        void setSkills(const std::vector<const Skill*>& skills);
        void recalculateSkillsAvailability();
        void resetScroller();

    private:
        SkillBars_t _skillBars;

        sf::Texture* _textureBar{nullptr};
        sf::Texture* _textureLocked{nullptr};
        sf::Font* _font{nullptr};

        WindowWidget* _border{nullptr};
        ScrollAreaWidget* _skillsScroller{nullptr};
        ListWidget* _skillsList{nullptr};

        void _initNewSkillBarsAmount(size_t newAmount);
    };
}