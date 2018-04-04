#include "SkillCodex.hpp"

namespace rat
{

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

    
    

}