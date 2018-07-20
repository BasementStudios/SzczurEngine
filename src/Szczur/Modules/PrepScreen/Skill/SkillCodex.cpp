#include "SkillCodex.hpp"

#include <fstream>

#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{

    SkillCodex::SkillCodex()
    {
        _initSkills();   
    }
    

    Skill* SkillCodex::getSkill(const std::string& name)
    {
        auto found = _skills.find(name);
        if(found == _skills.end()) return nullptr;
        auto& ptr = found->second;

        return ptr.get();
    }

    Skill* SkillCodex::addSkill(std::unique_ptr<Skill> skill)
    {
        const std::string& name = skill->getName();
        Skill* ptr = skill.get();
        _skills.emplace(name, std::move(skill));
        return ptr;
    }

    void SkillCodex::_initSkills()
    {
        /*
        auto skill = std::make_unique<Skill>("Fire Strike");
        skill->setProfession("Range");
        skill->setTexturePath("test2.png");
        skill->setPPCost(3);
        skill->setRequirements({
            {GlyphID::Wrath, 1}
        });
        _skills.emplace(skill->getName(), std::move(skill));

        skill = std::make_unique<Skill>("Rain Of Fire");
        skill->setProfession("Range");
        skill->setTexturePath("test1.png");
        skill->setPPCost(2);
        skill->setRequirements({
            {GlyphID::Wrath, 1}
        });
        _skills.emplace(skill->getName(), std::move(skill));

        skill = std::make_unique<Skill>("Fire Bee");
        skill->setProfession("Range");
        skill->setTexturePath("test4.png");
        skill->setPPCost(3);
        skill->setRequirements({
            {GlyphID::Wrath, 1}
        });
        _skills.emplace(skill->getName(), std::move(skill));

        skill = std::make_unique<Skill>("Fire Shards");
        skill->setProfession("Mele");
        skill->setTexturePath("test5.png");
        skill->setPPCost(2);
        skill->setRequirements({
            {GlyphID::Wrath, 1},
            {GlyphID::Fear, 1}
        });
        _skills.emplace(skill->getName(), std::move(skill));

        skill = std::make_unique<Skill>("Zanger");
        skill->setProfession("Mele");
        skill->setTexturePath("test8.png");
        skill->setPPCost(5);
        skill->setRequirements({
            {GlyphID::Wrath, 1}
        });
        _skills.emplace(skill->getName(), std::move(skill));

        skill = std::make_unique<Skill>("Shadow Fiend");
        skill->setProfession("Range");
        skill->setTexturePath("test3.png");
        skill->setPPCost(3);
        skill->setRequirements({
            {GlyphID::Wrath, 1}           
        });
        _skills.emplace(skill->getName(), std::move(skill));

        skill = std::make_unique<Skill>("Earth Strike");
        skill->setProfession("Range");
        skill->setTexturePath("test7.png");
        skill->setPPCost(5);
        skill->setRequirements({
            {GlyphID::Wrath, 1}
        });
        _skills.emplace(skill->getName(), std::move(skill));

        skill = std::make_unique<Skill>("Earth Elemental");
        skill->setProfession("Range");
        skill->setTexturePath("test6.png");
        skill->setPPCost(3);
        skill->setRequirements({
            {GlyphID::Wrath, 1}
        });
        _skills.emplace(skill->getName(), std::move(skill));
        */

       nlohmann::json j;
       std::ifstream in("Assets/PrepScreen/skills.json");
       in >> j;
       loadFromJson(j);


    }

    void SkillCodex::loadFromJson(nlohmann::json& j)
    {
        if(j.find("iconsPath") != j.end()) _mainPath = j["iconsPath"];
        _loadSkills(j["skills"]);
    }

    void SkillCodex::_loadSkills(nlohmann::json& j)
    {
        _skills.clear();
        for(auto it = j.begin(); it != j.end(); ++it)
        {
            auto skill = std::make_unique<Skill>(it.value());
            skill->setStringID(it.key());
            addSkill(std::move(skill));
        }
    }

    void SkillCodex::initAssetsViaGUI(GUI& gui)
    {
        for(auto& [name, skill] : _skills)
        {
            const auto& path = _mainPath + skill->getTexturePath();
            gui.addAsset<sf::Texture>(path);
            skill->setTexture(gui.getAsset<sf::Texture>(path));
        }
    }
}