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
    class Widget; class ScrollAreaWidget; class ImageWidget;

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

    private:
        SkillCodex _codex;
        ResourcesContainer _source;

        void test();

        Widget* _base{nullptr};

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
    };
}