#include "Quest.hpp"

#include <Json/json.hpp>

#include "Szczur/Utility/Logger.hpp"

#include "Szczur/Modules/QuestLog/QuestLog.hpp"

using namespace nlohmann;

using namespace nlohmann;

namespace rat
{
    Quest::Quest(QuestLog& owner, const std::string& name)
    :
    _owner(owner),
    _name(name)
    {
        _rootNode = new QuestNode(this, "Root");
    }
    QuestNode* Quest::getNode(const std::string& nodeName)
    {
        auto found = _nodes.find(nodeName);
        if(found == _nodes.end()) return nullptr;

        std::cout << "Looked for: " << nodeName << " Found: " << found->second->getName() << "\n";

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
        node->setName(nodeName);
        _nodes.emplace(nodeName, std::move(node));
    }
    QuestNode* Quest::getRoot()
    {
        return _rootNode;
    }
    void Quest::start()
    {
        std::cout << "Quest Started\n";
        _state = State::Active;
        _rootNode->start();
    }
    void Quest::finish()
    {
        _state = State::Finished;
        std::cout << "Jo Jo, you finished Quest\n";
    }
/*
    void Quest::setTitle(const TitleInfo& info)
    {      
        auto* gui = _owner.getGUI();
        gui->resetSubtitles();

        gui->setTitle(info);
    }
        
    QuestTitle* Quest::addSubtitle(const TitleInfo& info)
    {
        //if(_state != State::Active) return nullptr;
        auto* gui = _owner.getGUI();
        return gui->addSubtitle(info);
    }
*/

    json Quest::getJson() const
    {
        json j;
        
        j["state"] = static_cast<int>(_state);
        j["reqs"] = _reqs.getJson();

        j["root"] = _rootNode->getJson();

        return j;
    }
    void Quest::loadFromJson(nlohmann::json& j)
    {
        _state = static_cast<State>(j["state"]);

        _loadReqsFromJson(j["reqs"]);
        _loadRootFromJson(j["root"]);
    }

    void Quest::_loadReqsFromJson(nlohmann::json& j)
    {
        _reqs.resetValues();
        _reqs.loadFromJson(j);
    }

    void Quest::_loadRootFromJson(nlohmann::json& j)
    {
        _resetNodesReqs();
        _rootNode->loadFromJson(j);
        /* 
        if(_state == State::Active)
        {
            _activateRootsGUI();
        }
        */
    }

    void Quest::_resetNodesReqs()
    {
        for(auto& [name, node] : _nodes)
        {
            node->reset();
        }
    }
/*
    void Quest::_activateRootsGUI()
    {
        auto gui = _owner.getGUI();
        gui->resetSubtitles();
        _rootNode->resume();
    }
*/
    void Quest::setName(const std::string& name)
    {
        _name = name;
    }
    const std::string& Quest::getName() const
    {
        return _name;
    }

    Requirements& Quest::getReqs()
    {
        return _reqs;
    }

    void Quest::initScript(Script& script) {
    auto object = script.newClass<Quest>("Quest", "QuestLog");

    // Main
    object.set("getNode", &Quest::getNode);
    //object.set("setTitle", &Quest::setTitle);
    object.set("getRoot", &Quest::getRoot);
    object.set("getReqs", &Quest::getReqs);

    object.init();
}
}