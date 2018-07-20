#include "PrepScreen.hpp"

#include <cassert>
#include <fstream>

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
        Script& script = getModule<Script>();
        auto module = script.newModule("PrepScreen");

        module.set_function("hide", &PrepScreen::hide, this);
        module.set_function("show", &PrepScreen::show, this);
        module.set_function("getSelectedSkills", &PrepScreen::getSelectedSkills, this);
        module.set_function("showOnlyEssenceOrbs", &PrepScreen::showOnlyEssenceOrbs, this);
        module.set_function("clearEnemies", &PrepScreen::clearEnemies, this);
        module.set_function("pushEnemy", &PrepScreen::pushEnemy, this);
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

    //    _leftList = new ListWidget;
    //    _leftList->setPropSize(0.463f, 1.f);
    //    _leftList->setPropBetweenPad(0.04f);
    //    _leftList->setPropPosition(0.f, 0.f);
    //    _base->add(_leftList);

        _profArea.setParent(_base);
        _profArea.setPropPosition(0.f, 0.f);


        auto* skillList = new ListWidget;
        skillList->makeHorizontal();
        // skillList->setPropSize(0.463f, 0.f);
        // skillList->setPropSize(0.38, 0.f);
        // skillList->setAutoBetweenPadding();
        // skillList->setPropPosition(0.f, 0.2f);
        // _base->add(skillList);


        _skillArea.setParent(_base);
        _skillArea.setPropPosition(0.f, 0.7f);
        _glyphArea.setParent(_base);
        _glyphArea.setPropPosition(0.1f, 0.7f);

        _ppBack = new WindowWidget;
        _base->add(_ppBack);
        _ppBack->setPropSize(0.75f, 0.14f);
        _ppBack->setMainPatchPropSize({0.3f, 0.3f});
        _ppBack->setPropPosition(0.5f, -0.07f);
        _ppBack->makeChildrenUnresizable();
        _ppBack->setColor({255, 255, 255, 170});

        _grayPPArea.setParent(_ppBack);
        _grayPPArea.setPropPosition(0.5f, 0.47f);
        _chosenSkillArea.setParent(_ppBack);

        _rightList = new ListWidget;
        _rightList->setPropSize(0.463f, 1.f);
        _rightList->setPropPosition(1.f, 0.f);
        _base->add(_rightList);

        _battleButton.setParent(_rightList);
        _enemyArea.setParent(_rightList);



        _initAssetsViaGUI();

        _ppBack->setTexture(gui.getTexture("Assets/PrepScreen/BackgroundEQ.png"), 30);

        test();
    }

    void PrepScreen::show()
    {
        _base->fullyActivate();

        _profArea.fullyActivate();
        _skillArea.fullyActivate();
        _glyphArea.fullyActivate();
        _glyphArea.setPosToCenter();

        _rightList->fullyActivate();
        _ppBack->fullyActivate();
    }
    void PrepScreen::hide()
    {
        _base->fullyDeactivate();
    }

    std::vector<std::string> PrepScreen::getSelectedSkills() const
    {
        std::vector <std::string> ids;
        ids.reserve (_boughtSkills.size ());
        for (const auto& skill : _boughtSkills)
        {
            ids.emplace_back (skill->getStringID ());
        }
        return ids;
    }

    void PrepScreen::showOnlyEssenceOrbs()
    {
        _base->fullyActivate();

        _profArea.fullyDeactivate();
        _skillArea.fullyDeactivate();

        _rightList->fullyDeactivate();
        _ppBack->fullyDeactivate();

        _glyphArea.fullyActivate();
        _glyphArea.moveToLeft();
    }

    void PrepScreen::loadEnemiesFromJson(nlohmann::json& j)
    {
        _enemyCodex.loadFromJson(j);

        auto& gui = getModule<GUI>();
        _enemyCodex.initAssetsViaGUI(gui);
    }

    void PrepScreen::clearEnemies()
    {
        _enemyArea.clear();
    }
    void PrepScreen::pushEnemy(const std::string& nameID)
    {
        auto* enemy = _enemyCodex.getEnemy(nameID);
        if(!enemy)
        {
            LOG_ERROR("Cannot find enemy \"", nameID, "\"");
            return;
        }

        _enemyArea.pushEnemy(enemy, getModule<GUI>());
    }


    void PrepScreen::test()
    {
        auto& gui = getModule<GUI>();

        setMaximumPPSlotsAmount(10);
        unlockPPSlots(8);
        fillPP(8);

        nlohmann::json j;
        std::ifstream in("Assets/Pawns/pawns.json");
        if(!in.good()) std::cout << "Nie Wgralo pliku enemies\n";
        in >> j;

        loadEnemiesFromJson(j);

        addGlyph(GlyphID::Wrath);
        addGlyph(GlyphID::Wrath);
        addGlyph(GlyphID::Wrath);

        addGlyph(GlyphID::Wearines);
        addGlyph(GlyphID::Wearines);
        addGlyph(GlyphID::Wearines);

        addGlyph(GlyphID::Fear);
        addGlyph(GlyphID::Fear);
        addGlyph(GlyphID::Fear);
        
        addGlyph(GlyphID::Desperation);
        addGlyph(GlyphID::Desperation);
        addGlyph(GlyphID::Desperation);


        setProfession("Range");
        hide();
    }

    void PrepScreen::setCallback(const std::function<void(Widget*)>& func )
    {
        _battleButton.setCallback(func);
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