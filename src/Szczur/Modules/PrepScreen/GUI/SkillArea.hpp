#pragma once

#include "SkillBar.hpp"
#include "../SkillCodex.hpp"
#include "../SortedSkillsContainer.hpp"

#include <unordered_map>
#include <set>
#include <vector>
#include <memory>

namespace rat
{
    class GUI; class ScrollAreaWidget;
    class ChosenSkillArea;

    class SkillArea
    {
        using SkillBars_t = std::vector<std::unique_ptr<SkillBar>>;
    public:
        SkillArea(GrayPPBar& sourceBar);

        void initAssetsViaGUI(GUI& gui);
        void initViaSkillCodex(SkillCodex& skillCodex);

        void setParent(Widget* parent);

        void activate();
        void activate(const std::string& profession, const std::set<std::string>& colors);
        void deactivate();
        void setColors(const std::set<std::string>& colors);
        void addColor(const std::string& color);
        void removeColor(const std::string& color);
        void setProfession(const std::string& profession);
        void initChosenSkillArea(ChosenSkillArea& chosenSkillArea);
        ChosenSkillArea& getChosenSkillArea() const;
        void recalculate();

        GrayPPBar& getSourceBar();

    private:
        GrayPPBar& _sourceBar;
        ChosenSkillArea* _chosenSkillArea{nullptr};
        SortedSkillsContainer _skills;
        SkillBars_t _skillBars;
        size_t _activeBarsAmount{0};

        sf::Texture* _textureBar{nullptr};
        sf::Texture* _textureLocked{nullptr};
        sf::Font* _font{nullptr};

        ScrollAreaWidget* _base{nullptr};

        void _initNewSkillBarsAmount(size_t newAmount);

        float _barHeight{80.f};

        std::set<std::string> _chosenColors;
        std::string _curentProfession;
    };
}