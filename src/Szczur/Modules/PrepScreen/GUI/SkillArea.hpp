#pragma once

#include "SkillBar.hpp"
#include "../SkillCodex.hpp"

#include <unordered_map>
#include <vector>
#include <memory>

namespace rat
{
    class GUI; class ScrollAreaWidget;

    using SkillBars_t = std::vector<std::unique_ptr<SkillBar>>;
    using SkillColors_t = std::unordered_map<std::string, SkillBars_t>;
    using SkillProfessions_t = std::unordered_map<std::string, SkillColors_t>;

    class SkillArea
    {
    public:
        SkillArea(GrayPPBar& sourceBar);

        void initAssetsViaGUI(GUI& gui);
        void initViaSkillCodex(SkillCodex& skillCodex);

        void setParent(Widget* parent);

        void activate(const std::string& profession, const std::string& color);
        void hideAll();
        void setColor(const std::string& color);
        void setProfession(const std::string& profession);

    private:
        GrayPPBar& _sourceBar;
        SkillProfessions_t _skillBars;
        
        sf::Texture* _textureBar{nullptr};
        sf::Texture* _textureLocked{nullptr};

        ScrollAreaWidget* _base{nullptr};

        void _addSkillBar(Skill* skill);
        void _hide(const std::string& profession, const std::string& color);
        void _active(const std::string& profession, const std::string& color);

        float _barHeight{80.f};

        std::string _curentColor;
        std::string _curentProfession;
    };
}