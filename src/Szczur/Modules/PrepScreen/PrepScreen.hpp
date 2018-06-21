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
#include "GUI/SkillArea.hpp"
//#include "GUI/ColoredPPBar.hpp"
#include "GUI/GrayPPArea.hpp"
#include "GUI/ProfessionArea.hpp"
#include "GUI/ChosenSkillArea.hpp"
#include "GUI/ColorFilterArea.hpp"

#include "GUI/EnemyList/EnemyArea.hpp"

#include "GUI/InfoBar.hpp"
#include "GUI/GlyphArea.hpp"

#include "Enemy/EnemyCodex.hpp"

namespace rat
{
    class InterfaceWidget; class ScrollAreaWidget; class ImageWidget;

    class PrepScreen : public Module <Window, Input, Script, GUI>
    {
    public:
        void init()
        {
            initGUI();
        }

        void initScript();
        void initGUI();

        PrepScreen();
        ~PrepScreen();

        SkillCodex& getSkillCodex();

        void addPP(int amount = 1);
        void takePP(int amount = 1);
        bool hasPPAmount(int amount) const;

        void addGlyph(GlyphID glyphID);
        void removeGlyph(GlyphID glyphID);
        void activateGlyph(GlyphID glyphID);
        void deactivateGlyph(GlyphID glyphID);
        bool hasEnoughPowerfulGlyph(GlyphID glyphID, size_t powerLevel) const;

    private:
        SkillCodex _codex;
        ResourcesContainer _source;

        void test();

        InterfaceWidget* _base{nullptr};

        GrayPPArea _grayPPArea;
        SkillArea _skillArea;
        ProfessionArea _profArea;
        ChosenSkillArea _chosenSkillArea;
        //ColorFilterArea _colorFilterArea;

        InfoBar _infoBar;

        WindowWidget* _testBar{nullptr};
        GlyphArea _testGlyphBar;

        EnemyArea _enemyArea;
        EnemyCodex _enemyCodex;

        void _initProfessionBar(GUI& gui);
        void _initSkillArea();
        void _initProfArea();
        void _initChosenSkillArea();
        void _initColorFilterArea();

        void _initEnemyArea();

        void _loadAssetsFromGUI();

        void _calcGlyphGUI(GlyphID id);
        void _calcPPsGUI();
        void _calcSkillsGUI();
    };
}