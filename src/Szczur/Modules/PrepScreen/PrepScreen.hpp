//#include "Szczur/Utility/Modules/Module.hpp"
#pragma once
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

#include "Szczur/Modules/PrepScreen/ResourcesContainer.hpp"
#include "Szczur/Modules/PrepScreen/Skill/SkillCodex.hpp"
#include "GUI/Skills/List/SkillArea.hpp"
#include "GUI/PPS/Gray/GrayPPArea.hpp"
#include "GUI/Professions/ProfessionArea.hpp"
#include "GUI/Skills/Chosen/ChosenSkillArea.hpp"

#include "GUI/EnemyList/EnemyArea.hpp"
#include "GUI/PPS/Glyphes/GlyphArea.hpp"

#include "Szczur/Modules/PrepScreen/Skill/SortedSkillsContainer.hpp"

#include "Enemy/EnemyCodex.hpp"

namespace rat
{
    class InterfaceWidget; class ScrollAreaWidget; class ImageWidget;

    class PrepScreen : public Module <Window, Input, Script, GUI>
    {
    public:
        void init()
        {
            _sortedSkills.initViaSkillCodex(_codex);
            initGUI();
        }

        void initScript();
        void initGUI();

        PrepScreen();
        ~PrepScreen();

        SkillCodex& getSkillCodex();

        void setMaximumPPSlotsAmount(int amount);
        int getMaximumPPSlotsAmount() const;

        void unlockPPSlots(int amount = 1);
        void lockPPSlots(int amount = 1);
        void setUnlockedPPSlots(int amount);
        bool hasFilledPPAmount(int amount) const;

        void fillPP(int amount);
        void emptyPP(int amount);
        void setFilledPPAmount(int amount);
        int getFilledPPAmount() const;


        void addGlyph(GlyphID glyphID);
        void removeGlyph(GlyphID glyphID);
        bool canBeGlyphActivated(GlyphID glyphID) const;
        void activateGlyph(GlyphID glyphID);
        bool canBeGlyphDeactivated(GlyphID glyphID) const;
        void deactivateGlyph(GlyphID glyphID);
        bool hasEnoughPowerfulGlyph(GlyphID glyphID, size_t powerLevel) const;

        void buySkill(const Skill* skill);
        bool canSkillBeBought(const Skill* skill) const;
        void returnSkill(const Skill* skill);
        bool isSkillBought(const Skill* skill) const;

        void setProfession(const std::string& profession);

        void dimPPsNeededToBuySkill(const Skill* skill);
        void normPPsNeededToBuySkill(const Skill* skill);

    private:
        SkillCodex _codex;
        ResourcesContainer _source;
        SortedSkillsContainer _sortedSkills;

        void test();

        InterfaceWidget* _base{nullptr};

        GrayPPArea _grayPPArea;
        SkillArea _skillArea;
        ProfessionArea _profArea;
        ChosenSkillArea _chosenSkillArea;
        GlyphArea _glyphArea;

        bool _isAnyBoughtSkillNeedGlyph(GlyphID glyphID, size_t power) const;

        std::set<const Skill*> _boughtSkills;
        const Skill* _dimedPPsSkill{nullptr};

        std::string _currentProf;


        EnemyArea _enemyArea;
        EnemyCodex _enemyCodex;

        void _loadAssetsFromGUI();
        void _initAssetsViaGUI();

        void _calcGlyphGUI(GlyphID id);
        void _calcPPsGUI();
        void _calcSkillsGUI();
    };
}