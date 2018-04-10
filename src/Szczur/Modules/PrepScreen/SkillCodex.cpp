#include "SkillCodex.hpp"

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
        auto skill = std::make_unique<Skill>("Earth Strike");
        skill->setProfession("Mele");
        skill->setTexturePath("assets/PrepScreen/test2.png");
        skill->setPPCost(5);
        skill->setRequirements({
            {"Earth", 1}
        });
        _skills.emplace(skill->getName(), std::move(skill));

        skill = std::make_unique<Skill>("Rain Of Fire");
        skill->setProfession("Aura");
        skill->setTexturePath("assets/PrepScreen/test1.png");
        skill->setPPCost(7);
        skill->setRequirements({
            {"Fire", 1}
        });
        _skills.emplace(skill->getName(), std::move(skill));
    }
    
    

}