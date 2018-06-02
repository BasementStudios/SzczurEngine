#pragma once

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

#include <memory>
#include <unordered_map>

#include <Json/json.hpp>

#include "Quest/Quest.hpp"
#include "GUI/QuestGUI.hpp"

#include "Quest/Requirements.hpp"

namespace rat 
{
    class Widget;
    class QuestLog : public Module<Input, Window, Script, GUI>
    {
        using Quest_t = std::unique_ptr<Quest>;
    public:
        QuestLog();
        void init();

        void update(float dt);
        void render();

        void setTitle(const std::string& name);

        void _resetWidget(Widget*);

        void addQuest(std::string name);
        Quest* getQuest(const std::string name);

        void addQuestFrom(std::string name, Quest_t quest);

        QuestGUI* getGUI();

        void load(const std::string& path);
        void save(const std::string& path) const;

        nlohmann::json getJson() const;
        void loadFromJson(nlohmann::json& j);

        Requirements& getReqs();

        void initScript();

    private:

        std::unique_ptr<Quest> _quest;
        Widget* _widget{nullptr};
        std::unique_ptr<QuestGUI> _title;

        std::unordered_map<std::string, Quest_t> _quests;

        Requirements _reqs;

        void _debugCounterTest();
    };

}