//#include "Szczur/Utility/Modules/Module.hpp"
#pragma once
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

//#include "Szczur/Modules/PrepScreen/PPContainer.hpp"
#include "Szczur/Modules/PrepScreen/SkillCodex.hpp"
#include "GUI/SkillArea.hpp"
#include "GUI/ColoredPPBar.hpp"
#include "GUI/GrayPPBar.hpp"
#include "GUI/ProfessionArea.hpp"
#include "GUI/ChosenSkillArea.hpp"

namespace rat
{
    class Widget; class ScrollAreaWidget; class ImageWidget;

    class PrepScreen : public Module <Window, Input, Script, GUI>
    {
    public:
        void init()
        {
            //Script& script = getModule<Script>();
            //script.scriptFile("D:/Sczur/SzczurEngine/src/Szczur/Modules/PrepScreen/PrepScreenGUI/PSGUI.lua");
            initGUI();
        }

        void initScript();
        void initGUI();

        PrepScreen()
        :
        _coloredPPsBar(_source),
        _grayPPsBar(_source),
        _skillArea(_grayPPsBar),
        _profArea(_skillArea),
        _chosenSkillArea(_skillArea)
        {
            LOG_INFO(this, " : Module PrepScreen initing...");
            init();
            LOG_INFO(this, " : Module PrepScreen inited.");
        }
        ~PrepScreen()
        {
            LOG_INFO(this, " : Module PrepScreen destroyed.");
        }

        SkillCodex& getSkillCodex();

    private:
        SkillCodex _codex;
        PPContainer _source;
        
        void addSkill(Skill* skill);
        void test();
        std::vector<std::unique_ptr<Skill>> testSkills;

        sf::Texture* getTextureFrom(const std::string& name, GUI& gui);
        std::string _folder = "assets/PrepScreen/";

        Widget* _base{nullptr};
        ImageWidget* _coloredPPBar{nullptr};
        Widget* _grayPPBar{nullptr};

        ImageWidget* _centerWindow{nullptr};

        ScrollAreaWidget* _skillsBar{nullptr};

        ImageWidget* _eq{nullptr};
        ImageWidget* _chosenSkillsBar{nullptr};

        ColoredPPBar _coloredPPsBar;
        GrayPPBar _grayPPsBar;
        SkillArea _skillArea;
        ProfessionArea _profArea;
        ChosenSkillArea _chosenSkillArea;


        void _setWidgetSize(ImageWidget* widget, float x, float y);
        void _initProfessionBar(GUI& gui);
        void _initSkillArea();
        void _initProfArea();
        void _initChosenSkillArea();
    };
}