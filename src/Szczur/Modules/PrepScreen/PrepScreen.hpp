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
#include "GUI/SkillBar.hpp"
#include "GUI/ColoredPPBar.hpp"

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
    _coloredPPsBar(_source)
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
    ImageWidget* _grayPPBar{nullptr};

    ImageWidget* _centerWindow{nullptr};

    ScrollAreaWidget* _skillsBar{nullptr};

    ImageWidget* _eq{nullptr};
    ImageWidget* _chosenSkillsBar{nullptr};


    std::vector<std::unique_ptr<SkillBar>> _skills;
    ColoredPPBar _coloredPPsBar;
};
}