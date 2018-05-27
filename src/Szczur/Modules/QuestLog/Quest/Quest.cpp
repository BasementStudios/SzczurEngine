#include "Quest.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    Quest::Quest()
    {
        auto root = std::make_unique<QuestNode>(this);
        _rootNode = root.get(); 
        addNode("root", std::move(root));
    }
    QuestNode* Quest::getNode(const std::string& nodeName)
    {
        auto found = _namedNodes.find(nodeName);
        if(found == _namedNodes.end()) return nullptr;
        return found->second;
    }
    void Quest::addNode(const std::string& nodeName, Node_t node)
    {
        if(nodeName != "")
        {
            if(getNode(nodeName) != nullptr)
            {
                _namedNodes.emplace(nodeName, node.get());
            }
        }
        _wholeNodes.emplace_back(std::move(node));
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