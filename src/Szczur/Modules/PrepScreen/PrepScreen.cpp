#include "PrepScreen.hpp"
#include "Skill.hpp"

#include "GUI/SkillBar.hpp"

#include "ProfessionTypes.hpp"

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

        gui.addAsset<sf::Texture>("assets/PrepScreen/MenuBar.png");
        sf::Texture* menuBarTexture = gui.getAsset<sf::Texture>("assets/PrepScreen/MenuBar.png");

        _base = gui.addInterface();

        ImageWidget* ppBar = new ImageWidget;
        _base->add(ppBar);
        ppBar->setTexture(menuBarTexture);
        _setWidgetSize(ppBar, 1280.f, 120.f);

        _initProfessionBar(gui);
        
        _coloredPPBar = new ImageWidget;
        _grayPPBar = new Widget;
        _skillsBar = new ScrollAreaWidget;
        _chosenSkillsBar = new ImageWidget;

        ppBar->add(_coloredPPBar);
        //_coloredPPBar->setTexture(getTextureFrom("coloredPPBar.png", gui));

        ppBar->add(_grayPPBar);
        //_grayPPBar->setTexture(getTextureFrom("grayPPBar.png", gui));
        _grayPPBar->setPosition(0.f, 80.f);

        _centerWindow = new ImageWidget;
        _base->add(_centerWindow);
        _centerWindow->setTexture(menuBarTexture);
        _centerWindow->setPosition(200, 120);
        _setWidgetSize(_centerWindow, 760.f, 480.f);

        _skillsBar = new ScrollAreaWidget;
        _centerWindow->add(_skillsBar);
        _skillsBar->setPosition(40, 40);
        _skillsBar->setSize(240, 400);
        _skillsBar->setScrollSpeed(15);

        _eq = new ImageWidget;
        _base->add(_eq);
        _eq->setTexture(menuBarTexture);
        _eq->setPosition(0, 600);
        _setWidgetSize(_eq, 200.f, 120.f);

        _base->add(_chosenSkillsBar);
        _chosenSkillsBar->setTexture(menuBarTexture);
        _chosenSkillsBar->setPosition(200, 600);
        _setWidgetSize(_chosenSkillsBar, 760.f, 120.f);
        
        
        //mainWidget

        test();
    }


    void PrepScreen::test()
    {
        auto& gui = getModule<GUI>();

        std::cout << "Before PPsBar\n";
        _coloredPPsBar.setParent(_coloredPPBar);
        _coloredPPsBar.initTexturesViaGui(gui);
        _coloredPPsBar.setCenter(1280.f/2.f, 0.f);

        _grayPPsBar.setParent(_grayPPBar);
        _grayPPsBar.initTextureViaGui(gui);
        _grayPPsBar.setCenter(1280.f/2.f, 0);

        PPSource fireSource = {"Physical", 1, 10};
        //PPSource waterSource = {"Water", 1, 0};
        //PPSource airSource = {"Air", 1, 0};
        
        _source.addSource(fireSource);
        //_source.addSource(waterSource);
        
        _coloredPPsBar.recalculate();
        _grayPPsBar.recalculate();
        
        //_source.addSource(airSource);
        //_source.removeSource(fireSource);

        _coloredPPsBar.recalculate();
        _grayPPsBar.recalculate();

        _initSkillArea();
    }

    void PrepScreen::_setWidgetSize(ImageWidget* widget, float x, float y)
    {
        auto baseSize = static_cast<sf::Vector2f>(widget->getTexture()->getSize());
        widget->setScale({x/baseSize.x, y/baseSize.y}); 
    }

    void PrepScreen::_initProfessionBar(GUI& gui)
    {
        ImageWidget* _professionBar = new ImageWidget;
        _base->add(_professionBar);
        _professionBar->setTexture(gui.getAsset<sf::Texture>("assets/PrepScreen/MenuBar.png"));
        _professionBar->setPosition(0, 120);
        _setWidgetSize(_professionBar, 200.f, 480.f);

        ProfessionTypes profTypes;
        size_t i = 0;
        for(auto& profession : profTypes)
        {
            ImageWidget* profBar = new ImageWidget;
            _professionBar->add(profBar);
            profBar->setTexture(getTextureFrom(profession + "Icon.png", gui));
            profBar->setPosition(40.f, 120.f * i++);
            _setWidgetSize(profBar, 120.f, 120.f);    
        }
    }

    void PrepScreen::_initSkillArea()
    {
        _skillArea.setParent(_centerWindow);
        _skillArea.initViaSkillCodex(_codex);
        _skillArea.initAssetsViaGUI(getModule<GUI>());
    }
    

}