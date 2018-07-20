#pragma once
#include <unordered_map>
#include <memory>
#include <vector>

#include <Json/json.hpp>

#include "PrepSkill.hpp"


namespace rat 
{
    class GUI;

    class SkillCodex
    {

    public:
        SkillCodex();

        void reset();

        PrepSkill* getSkill(const std::string& name);
        PrepSkill* addSkill(std::unique_ptr<PrepSkill> skill);
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

        std::unordered_map<std::string, std::unique_ptr<PrepSkill>> _skills;
        std::string _mainPath = "Assets/PrepScreen/Icons/";
        void _initSkills();
    };
}