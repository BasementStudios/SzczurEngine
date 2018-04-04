#include "PrepScreen.hpp"
#include "Skill.hpp"

#include "GUI/SkillBar.hpp"

#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"

namespace rat
{
    void PrepScreen::initScript()
    {
        Script& script = getModule<Script>();
        auto module = script.newModule("PrepScreen");

        SCRIPT_SET_MODULE(PrepScreen, getSkillCodex);

    }

    SkillCodex& PrepScreen::getSkillCodex()
    {
        return _codex;
    }

    sf::Texture* PrepScreen::getTextureFrom(const std::string& name, GUI& gui)
    {
        std::string path = _folder + name;
        gui.addAsset<sf::Texture>(path);
        return gui.getAsset<sf::Texture>(path);
    }

    void PrepScreen::initGUI()
    {
        auto& gui = getModule<GUI>();

        _base = gui.addInterface();

        ImageWidget* ppBar = new ImageWidget;
        _base->add(ppBar);
        ppBar->setTexture(getTextureFrom("ppBar.png", gui));

        
        _coloredPPBar = new ImageWidget;
        _grayPPBar = new ImageWidget;
        _skillsBar = new ScrollAreaWidget;
        _chosenSkillsBar = new ImageWidget;
        ImageWidget* _professionBar = new ImageWidget;

        ppBar->add(_coloredPPBar);
        _coloredPPBar->setTexture(getTextureFrom("coloredPPBar.png", gui));

        ppBar->add(_grayPPBar);
        _grayPPBar->setTexture(getTextureFrom("grayPPBar.png", gui));
        _grayPPBar->setPosition(0.f, 80.f);

        _base->add(_professionBar);
        _professionBar->setTexture(getTextureFrom("professionBar.png", gui));
        _professionBar->setPosition(0, 120);

        ImageWidget* mele = new ImageWidget;
        ImageWidget* range = new ImageWidget;
        ImageWidget* aura = new ImageWidget;
        ImageWidget* iner = new ImageWidget;

        _professionBar->add(mele);
        mele->setTexture(getTextureFrom("meleIcon.png", gui));
        mele->setPosition(40, 0);

        _professionBar->add(range);
        range->setTexture(getTextureFrom("rangeIcon.png", gui));
        range->setPosition(40, 120);

        _professionBar->add(aura);
        aura->setTexture(getTextureFrom("auraIcon.png", gui));
        aura->setPosition(40, 240);

        _professionBar->add(iner);
        iner->setTexture(getTextureFrom("inerFireIcon.png", gui));
        iner->setPosition(40, 360);

        _centerWindow = new ImageWidget;
        _base->add(_centerWindow);
        _centerWindow->setTexture(getTextureFrom("centerWindow.png", gui));
        _centerWindow->setPosition(200, 120);

        _skillsBar = new ScrollAreaWidget;
        _centerWindow->add(_skillsBar);
        _skillsBar->setPosition(40, 40);
        _skillsBar->setSize(240, 400);
        _skillsBar->setScrollSpeed(15);

        _eq = new ImageWidget;
        _base->add(_eq);
        _eq->setTexture(getTextureFrom("eq.png", gui));
        _eq->setPosition(0, 600);

        _base->add(_chosenSkillsBar);
        _chosenSkillsBar->setTexture(getTextureFrom("chosenSkills.png", gui));
        _chosenSkillsBar->setPosition(200, 600);
        
        //mainWidget

        test();
    }

    void PrepScreen::addSkill(Skill* skill)
    {
        auto newSkill = std::make_unique<SkillBar>(_source);
        newSkill->setSkill(skill);
        newSkill->setParent(_skillsBar);
        size_t n = _skills.size();
        newSkill->setPosition(0, float(n*80));

        auto& gui = getModule<GUI>();
        newSkill->setBarTexture(getTextureFrom("skillBar.png", gui), getTextureFrom("skillBarLocked.png", gui));

        _skills.emplace_back(std::move(newSkill));
    }

    void PrepScreen::test()
    {
        auto& gui = getModule<GUI>();

        auto test1 = std::make_unique<Skill>("testGreen");
        test1->setIcon(getTextureFrom("test1.png", gui));
        test1->setPPCost(3);
        test1->addRequirement("Earth");

        auto test2 = std::make_unique<Skill>("testRed");
        test2->setIcon(getTextureFrom("test2.png", gui));
        test2->setPPCost(5);
        test2->addRequirement("Fire");

        addSkill(test1.get());
        addSkill(test2.get());

        testSkills.emplace_back(std::move(test1));
        testSkills.emplace_back(std::move(test2));
    }

}