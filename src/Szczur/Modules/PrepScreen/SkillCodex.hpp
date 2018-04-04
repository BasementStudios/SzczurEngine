#pragma once
#include <unordered_map>
#include <memory>

#include "Skill.hpp"


namespace rat 
{
    class SkillCodex
    {

        public:
        Skill* getSkill(const std::string& name);
        Skill* addSkill(std::unique_ptr<Skill> skill);
        void renameSkill(const std::string& oldName, const std::string& newName);

        private:
        std::unordered_map<std::string, std::unique_ptr<Skill>> _skills;
    };
}