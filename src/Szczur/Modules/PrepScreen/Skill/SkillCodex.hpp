#pragma once
#include <unordered_map>
#include <memory>
#include <vector>

#include <nlohmann/json.hpp>

#include "Skill.hpp"


namespace rat 
{
    class GUI;

    class SkillCodex
    {

    public:
        SkillCodex();

        Skill* getSkill(const std::string& name);
        Skill* addSkill(std::unique_ptr<Skill> skill);
        void renameSkill(const std::string& oldName, const std::string& newName);
        void initAssetsViaGUI(GUI& gui);

        auto begin()
        {
            return _skills.begin();
        }
        auto end()
        {
            return _skills.end();
        }

        void loadFromJson(nlohmann::json& j);

    private:
        void _loadSkills(nlohmann::json& j);

        std::unordered_map<std::string, std::unique_ptr<Skill>> _skills;
        std::string _mainPath = "Assets/PrepScreen/Icons/";
        void _initSkills();
    };
}