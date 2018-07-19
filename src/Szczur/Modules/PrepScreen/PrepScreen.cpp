#include "PrepScreen.hpp"

#include <cassert>

#include "Skill/Skill.hpp"

#include "ProfessionTypes.hpp"

#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"

#include "GlyphTypes.hpp"

#include "Szczur/Utility/Logger.hpp" 

namespace rat
{

    PrepScreen::PrepScreen()
    :
    _grayPPArea(),
    _glyphArea(*this),
    _skillArea(*this),
    _profArea(*this),
    _chosenSkillArea(*this, 6)
    {
        LOG_INFO(this, " : Module PrepScreen initing...");
        init();
        LOG_INFO(this, " : Module PrepScreen inited.");
    }

    PrepScreen::~PrepScreen()
    {
        LOG_INFO(this, " : Module PrepScreen destroyed.");
    }

    void PrepScreen::initScript()
    {
    }

    void PrepScreen::setMaximumPPSlotsAmount(int amount)
    {
        _source.ppContainer.setMaximumSlotsAmount(amount);
        _calcPPsGUI();
        _calcSkillsGUI();
    }

    int PrepScreen::getMaximumPPSlotsAmount() const
    {
        _source.ppContainer.getMaximumSlotsAmount();
    }

    void PrepScreen::unlockPPSlots(int amount)
    {
        _source.ppContainer.unlockSlots(amount);

        _calcPPsGUI();
        _calcSkillsGUI();
    }
    void PrepScreen::lockPPSlots(int amount)
    {
        _source.ppContainer.lockSlots(amount);

        _calcPPsGUI();
        _calcSkillsGUI();
    }

    void PrepScreen::setUnlockedPPSlots(int amount)
    {
        _source.ppContainer.setUnlockedAmount(amount);

        _calcPPsGUI();
        _calcSkillsGUI();
    }

    void PrepScreen::fillPP(int amount)
    {
        emptyPP(-amount);
    }
    void PrepScreen::emptyPP(int amount)
    {
        _source.ppContainer.emptyPP(amount);

        _grayPPArea.setFilled(_source.ppContainer.getFilledPPAmount());
        _calcSkillsGUI();
    }
    void PrepScreen::setFilledPPAmount(int amount)
    {
        _source.ppContainer.setFilledPPAmount(amount);

        _grayPPArea.setFilled(_source.ppContainer.getFilledPPAmount());
        _calcSkillsGUI();
    }
    bool PrepScreen::hasFilledPPAmount(int amount) const
    {
        return _source.ppContainer.getFilledPPAmount() >= amount;
    }

    void PrepScreen::addGlyph(GlyphID glyphID)
    {
        _source.glyphContainer.addGlyph(glyphID);
        _calcGlyphGUI(glyphID);
        _calcSkillsGUI();
    }
    void PrepScreen::removeGlyph(GlyphID glyphID)
    {
        _source.glyphContainer.removeGlyph(glyphID);
        _calcGlyphGUI(glyphID);
        _calcSkillsGUI();
    }
    bool PrepScreen::canBeGlyphActivated(GlyphID glyphID) const
    {
        return _source.glyphContainer.getGlyphAmount(glyphID) < _source.glyphContainer.getGlyphTotalAmount(glyphID);
    }
    void PrepScreen::activateGlyph(GlyphID glyphID)
    {
        _source.glyphContainer.activateGlyph(glyphID);
        _calcGlyphGUI(glyphID);
        _calcSkillsGUI();
    }
    bool PrepScreen::canBeGlyphDeactivated(GlyphID glyphID) const
    {
        size_t glyphsPower = _source.glyphContainer.getGlyphAmount(glyphID);
        if(glyphsPower == 0) return false;
        if(_source.ppContainer.getFilledPPAmount() == 0) return false;
        if(_isAnyBoughtSkillNeedGlyph(glyphID, glyphsPower)) return false;
        return true;
    }
    bool PrepScreen::_isAnyBoughtSkillNeedGlyph(GlyphID glyphID, size_t power) const
    {
        auto oneThatNeed = std::find_if(_boughtSkills.begin(), _boughtSkills.end(), [&glyphID, &power](const Skill* skill){
            const auto& cost = skill->getCostInfo();
            return cost.needGlyphThatPower(glyphID, power);
        });

        return oneThatNeed != _boughtSkills.end();
    }
    void PrepScreen::deactivateGlyph(GlyphID glyphID)
    {
        _source.glyphContainer.deactivateGlyph(glyphID);
        _calcGlyphGUI(glyphID);
        _calcSkillsGUI();
    }
    bool PrepScreen::hasEnoughPowerfulGlyph(GlyphID glyphID, size_t powerLevel) const
    {
        return _source.glyphContainer.hasRequirements({{glyphID, powerLevel}});
    }

    void PrepScreen::buySkill(const Skill* skill)
    {
        assert(canSkillBeBought(skill));
        const auto& cost = skill->getCostInfo();
        emptyPP(int(cost.getCost()));

        skill->buy();
        _boughtSkills.emplace(skill);
        _chosenSkillArea.addSkill(skill);

        _calcPPsGUI();
        _calcSkillsGUI();
    }
    bool PrepScreen::canSkillBeBought(const Skill* skill) const
    {
        const auto& cost = skill->getCostInfo();
        if(cost.getCost() > _source.ppContainer.getFilledPPAmount()) return false;
        for(auto& [id, power] : cost)
        {
            if(!hasEnoughPowerfulGlyph(id, power)) return false;
        }
        if(!_chosenSkillArea.hasFreeSpace()) return false;
        return true;
    }
    void PrepScreen::returnSkill(const Skill* skill)
    {
        assert(skill->isBought());

        const auto& cost = skill->getCostInfo();
        fillPP(int(cost.getCost()));

        skill->unBuy();
        _boughtSkills.erase(skill);

        _chosenSkillArea.recalculate();

        _calcPPsGUI();
        _calcSkillsGUI();
    }

    void PrepScreen::setProfession(const std::string& profession)
    {
        if(_currentProf == profession)
        {
            _skillArea.resetScroller();
        }
        else
        {
            _profArea.unchooseAll();
            _currentProf = profession;
            auto skills = _sortedSkills.getWholeProfession(profession);
            _skillArea.setSkills(skills);

            _calcSkillsGUI();
        }
    }

    void PrepScreen::dimPPsNeededToBuySkill(const Skill* skill)
    {
        assert(canSkillBeBought(skill));
        auto& cost = skill->getCostInfo();

        size_t ppCost = cost.getCost();
        _grayPPArea.dimFilledPPs(ppCost);

        _dimedPPsSkill = skill;
    }
    void PrepScreen::normPPsNeededToBuySkill(const Skill* skill)
    {
        if(skill != _dimedPPsSkill) return;

        _grayPPArea.undimPPs();

        _dimedPPsSkill = nullptr;
    }

    SkillCodex& PrepScreen::getSkillCodex()
    {
        return _codex;
    }

    void PrepScreen::_calcGlyphGUI(GlyphID id)
    {
        size_t activated = _source.glyphContainer.getGlyphAmount(id);
        size_t total = _source.glyphContainer.getGlyphTotalAmount(id);
        _glyphArea.setGlyph(id, activated, total);
    }
    
    void PrepScreen::_calcPPsGUI()
    {
        int filled = _source.ppContainer.getFilledPPAmount();
        int slots = _source.ppContainer.getUnlockedSlotsAmount();
        int maximum = _source.ppContainer.getMaximumSlotsAmount();

        _grayPPArea.setMaximum(maximum);
        _grayPPArea.setUnlocked(slots);
        _grayPPArea.setFilled(filled);
    }
    
    void PrepScreen::_calcSkillsGUI()
    {
        _skillArea.recalculateSkillsAvailability();
    }

    void PrepScreen::initGUI()
    {
        auto& gui = getModule<GUI>();

        _loadAssetsFromGUI();

        auto* background = new ImageWidget;
        gui.addInterface()->add(background);
        background->setPropSize(1.777778f, 1.f);
        background->setPropPosition(0.5f, 0.5f);
        background->setTexture(gui.getTexture("Assets/PrepScreen/Background.png"));
        
        _base = gui.addInterface();
        _base->setPadding(20.f, 20.f);
        _base->setSizingWidthToHeightProportion(1.f);
        


       auto* mainList = new ListWidget;
       mainList->makeHorizontal();
       mainList->setAutoBetweenPadding();
       _base->add(mainList);
       mainList->setPropPosition(0.5f, 0.5f);

       auto* leftList = new ListWidget;
       leftList->setPropSize(0.463f, 1.f);
       leftList->setPropBetweenPad(0.04f);
       mainList->add(leftList);

        _profArea.setParent(leftList);

        auto* skillList = new ListWidget;
        skillList->makeHorizontal();
        // skillList->setPropSize(0.463f, 0.f);
        skillList->setPropSize(0.38, 0.f);
        skillList->setAutoBetweenPadding();
        leftList->add(skillList);


        _glyphArea.setParent(skillList);
        _skillArea.setParent(skillList);

        auto* infoList = new ListWidget;
        infoList->setPropSize(0.8517777777777777f, 1.f);
        infoList->setPropBetweenPad(0.005f);
        mainList->add(infoList);

        _grayPPArea.setParent(infoList);
        _chosenSkillArea.setParent(infoList);

        auto* monsterList = new ListWidget;
        monsterList->setPropSize(0.463f, 1.f);
        mainList->add(monsterList);

        _battleButton.setParent(monsterList);
        _enemyArea.setParent(monsterList);



        _initAssetsViaGUI();
        
        //_enemyArea.setParent(mainList);
        //_enemyArea.setPropPosition(0.f, 0.5f);
        test();
    }

    void PrepScreen::show()
    {
        _base->fullyActivate();
    }
    void PrepScreen::hide()
    {
        _base->fullyDeactivate();
    }


    void PrepScreen::test()
    {
        auto& gui = getModule<GUI>();

        setMaximumPPSlotsAmount(10);
        unlockPPSlots(6);
        fillPP(4);

        addGlyph(GlyphID::Wrath);
        addGlyph(GlyphID::Wrath);
        addGlyph(GlyphID::Wrath);
        addGlyph(GlyphID::Wearines);
        addGlyph(GlyphID::Wearines);
        addGlyph(GlyphID::Wearines);
        addGlyph(GlyphID::Fear);
        addGlyph(GlyphID::Fear);
        activateGlyph(GlyphID::Wrath);
        
        setProfession("Range");

        _battleButton.setCallback([](Widget*){
            std::cout << "Do walki!!111\n";
        });

        hide();
        show();
    }


    void PrepScreen::_loadAssetsFromGUI()
    {
        auto& gui = getModule<GUI>();

        std::string path = "Assets/Test/";

        gui.addAsset<sf::Texture>(path + "Bar.png");
        gui.addAsset<sf::Texture>(path + "ChosenSkillWindow.png");
        gui.addAsset<sf::Texture>(path + "Scroller.png");
        gui.addAsset<sf::Texture>(path + "ScrollerBar.png");
        gui.addAsset<sf::Texture>(path + "ScrollerBound.png");
        gui.addAsset<sf::Texture>(path + "Window.png");
        gui.addAsset<sf::Texture>(path + "Window2.png");
        gui.addAsset<sf::Texture>(path + "GlyphCircle.png");
        gui.addAsset<sf::Texture>(path + "GrayPPWindow.png");
        gui.addAsset<sf::Texture>("Assets/Test/ChosenSkill.png");
        gui.addAsset<sf::Texture>("Assets/PrepScreen/GrayPP.png");
        gui.addAsset<sf::Texture>("Assets/PrepScreen/PPSlots.png");
        gui.addAsset<sf::Texture>("Assets/PrepScreen/SkillBarFilter.png");
        gui.addAsset<sf::Texture>("Assets/PrepScreen/SkillBack.png");
        gui.addTexture("Assets/PrepScreen/ChosenSkillArea.png");
        gui.addTexture("Assets/PrepScreen/Background.png");
        gui.addTexture("Assets/PrepScreen/BackgroundEQ.png");
                
        gui.addTexture("Assets/PrepScreen/Glyphes/Background.png");
        gui.addTexture("Assets/PrepScreen/Glyphes/Border.png");

        GlyphTypes glyphTypes;
        for(auto& type : glyphTypes)
        {
            const std::string typePath = "Assets/PrepScreen/Glyphes/" + type + ".png";
            gui.addAsset<sf::Texture>(typePath);
        }

        gui.addAsset<sf::Font>("Assets/fonts/NotoMono.ttf");
        gui.addAsset<sf::Font>("Assets/fonts/anirm.ttf");
        gui.addFont("Assets/fonts/NotoSerif-Regular.ttf");

        _codex.initAssetsViaGUI(gui);
        _enemyCodex.initAssetsViaGUI(gui);
    }
    
    void PrepScreen::_initAssetsViaGUI()
    {
        auto& gui = getModule<GUI>();
        
        _grayPPArea.initAssetsViaGUI(gui);
        _glyphArea.initAssetsViaGUI(gui);
        _profArea.initAssetsViaGUI(gui);
        _skillArea.initAssetsViaGUI(gui);
        _chosenSkillArea.initAssetsViaGUI(gui);
        _enemyArea.initEnemiesViaCodex(_enemyCodex);
        _enemyArea.initAssetsViaGUI(gui);
        _battleButton.loadAssetsFromGUI(gui);
    }
    
}