#pragma once

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/QuestJournal/QuestJournal.hpp"

#include <memory>
#include <unordered_map>

#include <Json/json.hpp>

#include "Quest/Quest.hpp"
//#include "GUI/QuestGUI.hpp"

#include "Quest/Requirements.hpp"

namespace rat 
{
    class QuestLog : public Module<Input, Window, Script, QuestJournal>
    {
        using Quest_t = std::unique_ptr<Quest>;
    public:
        QuestLog();
        void init();

        void update(float dt);
        void render();

        void setTitle(const std::string& name);

        Quest* addQuest(std::string name);
        Quest* getQuest(const std::string name);

        //QuestGUI* getGUI();

        void load(const std::string& path);
        void save(const std::string& path) const;

        nlohmann::json getJson() const;
        void loadFromJson(nlohmann::json& j);

        Requirements& getReqs();

        void initScript();

        void activateQuest(const std::string& name);
        void addQuestTitle(const std::string& questName, int index);
        void addQuestDescription(const std::string& questName, int index);

    private:

        //std::unique_ptr<Quest> _quest;
        //Widget* _widget{nullptr};
        //std::unique_ptr<QuestGUI> _title;

        float timer = 0.f;
        bool toStep = false;
        int count = 0;

        std::unordered_map<std::string, Quest_t> _quests;

        Requirements _reqs;

        void _debugCounterTest();

        void testQuest();
    };

}