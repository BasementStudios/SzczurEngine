#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <fstream>

#include <Json/json.hpp>

#include "Szczur/Modules/Script/Script.hpp"

#include "QuestNode.hpp"
#include "Requirements.hpp"
//#include "Szczur/Modules/QuestLog/GUI/QuestInfoBar/QuestTitle.hpp"

namespace rat
{
    class QuestLog;
    class Quest
    {
        using Node_t = std::unique_ptr<QuestNode>;
    public:
        Quest(QuestLog& owner, const std::string& name = "");

        QuestNode* getNode(const std::string& nodeName);
        QuestNode* getRoot();
        void addNode(std::string nodeName, Node_t node); //specjalnie bez refa ;-;

        void start();
        void finish();

        //void setTitle(const TitleInfo& info);

        //QuestTitle* addSubtitle(const TitleInfo& info);

        void setName(const std::string& name);
        const std::string& getName() const;
        

        //void setSubtitle

        void setCurrentNode(QuestNode* current);

        void testLoad(std::ifstream& in);
        void testSave(std::ofstream& out);

        nlohmann::json getJson() const;
        void loadFromJson(nlohmann::json& j);

        Requirements& getReqs();

        static void initScript(Script& script);

    private:
        std::unordered_map<std::string, Node_t> _nodes;
        QuestNode* _rootNode;

        enum class State { NotStarted, Active, Inactive, Finished};
        State _state{State::NotStarted};
        std::string _name{""};
        
        QuestLog& _owner;

        void _loadRootFromJson(nlohmann::json& j);
        void _loadReqsFromJson(nlohmann::json& j);
        void _resetNodesReqs();

        //void _activateRootsGUI();

        Requirements _reqs;
    };
}