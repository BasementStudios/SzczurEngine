#include "Quest.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    Quest::Quest()
    {
        _rootNode = new QuestNode(this, "Root");
    }
    QuestNode* Quest::getNode(const std::string& nodeName)
    {
        auto found = _nodes.find(nodeName);
        if(found == _nodes.end()) return nullptr;
        return found->second.get();
    }
    void Quest::addNode(std::string nodeName, Node_t node)
    {
        bool needDefaultName = false;
        if(nodeName == "") needDefaultName = true;
        if(getNode(nodeName) != nullptr)
        {
            needDefaultName = true;
            LOG_INFO("Node with name \"", nodeName, "\" already exist, default name is set.");
        }

        if(needDefaultName)
        {
            nodeName = "N" + std::to_string(_nodes.size());
        }
        
        _nodes.emplace(nodeName, std::move(node));
    }
    QuestNode* Quest::getRoot()
    {
        return _rootNode;
    }
    void Quest::start()
    {
        std::cout << "Quest Started\n";
        _rootNode->start();
    }
    void Quest::finish()
    {
        std::cout << "Jo Jo, you finished Quest\n";
    }
}