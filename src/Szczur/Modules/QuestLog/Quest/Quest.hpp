#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

#include "QuestNode.hpp"
#include "Requirements.hpp"
#include "Szczur/Modules/QuestLog/GUI/QuestInfoBar/QuestTitle.hpp"

namespace rat
{
    class QuestLog;
    class Quest : public Requirements
    {
        using Node_t = std::unique_ptr<QuestNode>;
    public:
        Quest(QuestLog& owner);

        QuestNode* getNode(const std::string& nodeName);
        QuestNode* getRoot();
        void addNode(std::string nodeName, Node_t node); //specjalnie bez refa ;-;

        void start();
        void finish();

        void setTitle(const std::string& name);
        
        QuestTitle* addSubtitle(const std::string& name);
        QuestTitle* addSubtitle(const std::string& name, int current, int maximum);
        QuestTitle* addSubtitle(const std::string& name, bool isSuited);

        void setCurrentNode(QuestNode* current);
    private:
        std::unordered_map<std::string, Node_t> _nodes;
        QuestNode* _rootNode;
        
        QuestLog& _owner;
    };
}