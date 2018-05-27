#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

#include "QuestNode.hpp"

namespace rat
{
    using Node_t = std::unique_ptr<QuestNode>;
    class Quest
    {
    public:
        Quest();

        QuestNode* getNode(const std::string& nodeName);
        QuestNode* getRoot();
        void addNode(const std::string& nodeName, Node_t node);

        void start();
        void finish();
    private:
        std::unordered_map<std::string, QuestNode*> _namedNodes;
        std::vector<Node_t> _wholeNodes;
        QuestNode* _rootNode;
    };
}