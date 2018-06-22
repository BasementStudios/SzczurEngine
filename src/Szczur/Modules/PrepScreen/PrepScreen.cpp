#include "PrepScreen.hpp"

#include <cassert>

#include "Skill/Skill.hpp"

#include "GUI/SkillBar.hpp"

#include "ProfessionTypes.hpp"

#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"

#include "GlyphTypes.hpp"

#include "Szczur/Utility/Logger.hpp" 

namespace rat
{

    PrepScreen::PrepScreen()
    :
    _grayPPArea(),
    _testGlyphBar(*this),
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

    void PrepScreen::addPP(int amount)
    {
        if(amount < 0)
        {
            _source.ppContainer.remove(size_t(-amount));
        }
        else
        {
            _source.ppContainer.add(size_t(amount));
        }

        _calcPPsGUI();
        _calcSkillsGUI();
    }
    void PrepScreen::takePP(int amount)
    {
        if(amount < 0)
        {
            _source.ppContainer.returnTo(size_t(-amount));
        }
        else _source.ppContainer.takeFrom(size_t(amount));

        _calcPPsGUI();
        _calcSkillsGUI();
    }
    bool PrepScreen::hasPPAmount(int amount) const
    {
        return _source.ppContainer.getAmount() >= size_t(amount);
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
        if(_source.ppContainer.getAmount() == 0) return false;
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
        takePP(cost.getCost());

        skill->buy();
        _boughtSkills.emplace(skill);
        _chosenSkillArea.addSkill(skill);

        _calcPPsGUI();
        _calcSkillsGUI();
    }
    bool PrepScreen::canSkillBeBought(const Skill* skill) const
    {
        const auto& cost = skill->getCostInfo();
        if(cost.getCost() > _source.ppContainer.getAmount()) return false;
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
        takePP(-int(cost.getCost()));

        skill->unBuy();
        _boughtSkills.erase(skill);

        _chosenSkillArea.recalculate();

        //_chosenSkillArea.removeSkill(skill);
        //_skillArea.addSkill(skill);

        _calcPPsGUI();
        _calcSkillsGUI();
    }

    void PrepScreen::setProfession(const std::string& profession)
    {
        auto skills = _sortedSkills.getWholeProfession(profession);
        _skillArea.setSkills(skills);

        _calcSkillsGUI();
    }

    void PrepScreen::dimPPsNeededToBuySkill(const Skill* skill)
    {
        
        _dimedPPsSkill = skill;
    }
    void PrepScreen::normPPsNeededToBuySkill(const Skill* skill)
    {
        if(skill != _dimedPPsSkill) return;

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
        _testGlyphBar.setGlyph(id, activated, total);
    }
    
    void PrepScreen::_calcPPsGUI()
    {
        size_t active = _source.ppContainer.getAmount();
        size_t total = _source.ppContainer.getTotalAmount();

        _grayPPArea.setPPs(active, total);
    }
    
    void PrepScreen::_calcSkillsGUI()
    {
        _skillArea.recalculateSkillsAvailability();
    }

    void PrepScreen::initGUI()
    {
        auto& gui = getModule<GUI>();

        _loadAssetsFromGUI();
        _codex.initAssetsViaGUI(gui);

        _base = gui.addInterface();
        _base->setPadding(20.f, 20.f);
        _base->setSizingWidthToHeightProportion(1.f);

        test();
    }


    void PrepScreen::test()
    {
        auto& gui = getModule<GUI>();


        _grayPPArea.setParent(_base);
        _grayPPArea.initAssetsViaGUI(gui);
        _grayPPArea.setPosition(1280.f/2.f, 120);

        //_grayPPArea.recalculate();

        

        _initSkillArea(); // Traitor
        _initChosenSkillArea();
        _initProfArea();
        _initColorFilterArea();
        _initEnemyArea();

        _testGlyphBar.setPosition(1280.f/2.f, 10.f);
        _testGlyphBar.setParent(_base);
        _testGlyphBar.initAssetsViaGUI(gui);

        addPP(12);
        //takePP(3);

        addGlyph(GlyphID::Wrath);
        activateGlyph(GlyphID::Wrath);
        
        setProfession("Range");
    }

    void PrepScreen::_initSkillArea()
    {
        _skillArea.setParent(_base);
        _skillArea.setPosition(200.f, 150.f);
        _skillArea.initAssetsViaGUI(getModule<GUI>());
    }

    void PrepScreen::_initProfArea()
    {
        _profArea.setParent(_base);
        _profArea.setPosition(40.f, 150.f);
        _profArea.initAssetsViaGUI(getModule<GUI>());
    }
    
    void PrepScreen::_initChosenSkillArea()
    {
        _chosenSkillArea.setParent(_base);
        _chosenSkillArea.setPosition(200.f, 600.f);
        _chosenSkillArea.initAssetsViaGUI(getModule<GUI>());
    }
    void PrepScreen::_initColorFilterArea()
    {
        /*
        _colorFilterArea.setParent(_base);f_//coloredPPsBar(_source),
        _colorFilterArea.setPosition(500.f, 200.f);
        _colorFilterArea.initAssetsViaGUI(getModule<GUI>());
        _colorFilterArea.setWidth(250.f);   
        */
    }

    void PrepScreen::_initEnemyArea()
    {
        _enemyArea.setParent(_base);
        _enemyArea.setPosition(900.f, 150.f);
        _enemyArea.initEnemiesViaCodex(_enemyCodex);
        _enemyArea.initAssetsViaGUI(getModule<GUI>());
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
                

        GlyphTypes glyphTypes;
        for(auto& type : glyphTypes)
        {
            const std::string typePath = path + type + "Glyph.png";
            gui.addAsset<sf::Texture>(typePath);
        }

        gui.addAsset<sf::Font>("Assets/fonts/NotoMono.ttf");
        _enemyCodex.initAssetsViaGUI(gui);
    }
    
    
}