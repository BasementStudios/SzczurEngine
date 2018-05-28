#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

#include "QuestNode.hpp"
#include "Requirements.hpp"

namespace rat
{
    class Quest : public Requirements
    {
        using Node_t = std::unique_ptr<QuestNode>;
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