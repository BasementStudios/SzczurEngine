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
        if(found == _nodes.end())
        {
            LOG_ERROR("Cannot get step/node \"", nodeName, "\"");
            return nullptr;
        }

        return found->second.get();
    }
    void Quest::nextStep(const std::string& nodeName)
    {
        auto* node = getNode(nodeName);
        if(!node)
        {
            LOG_ERROR("Cannot next step \"", nodeName, "\"");
            return;
        }
        node->nextStep();
    }
    void Quest::addNode(std::string nodeName, Node_t node)
    {
        bool needDefaultName = false;
        if(nodeName == "") needDefaultName = true;
        if(_hasNode(nodeName))
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

    bool Quest::_hasNode(const std::string& nodeName) const
    {
        return _nodes.find(nodeName) != _nodes.end();
    }
    
    QuestNode* Quest::getRoot()
    {
        if(!_rootNode) LOG_ERROR("Root is nullptr...");
        return _rootNode;
    }
    void Quest::start()
    {
        std::cout << "Quest Started\n";
        _state = State::Active;
        _rootNode->activate();
        _rootNode->nextStep();
    }
    void Quest::finish()
    {
        _state = State::Finished;
        _owner.finishQuest(getName());
    }

    void Quest::setNameIndex(unsigned int index)
    {
        _nameIndex = index;
    }
    unsigned int Quest::getNameIndex() const
    {
        return _nameIndex;
    }

    void Quest::addTitle(int index)
    {
        _owner.addQuestTitle(getNameIndex(), index);
    }
    void Quest::addDescription(int index)
    {
        _owner.addQuestDescription(getNameIndex(), index);
    }

    json Quest::getSaveJson() const
    {
        json j;
        
        j["state"] = static_cast<int>(_state);
        j["reqs"] = _reqs.getJson();

        json ns;

        for(auto& [name, node] : _nodes)
        {
            ns.emplace(name, node->getSaveJson());
        }

        j["nodes"] = ns;

        //j["root"] = _rootNode->getJson();

        return j;
    }
    void Quest::loadFromSaveJson(nlohmann::json& j)
    {
        _state = static_cast<State>(j["state"]);

        _loadReqsFromJson(j["reqs"]);
        _loadNodesFromJson(j["nodes"]);
    }

    void Quest::_loadReqsFromJson(nlohmann::json& j)
    {
        _reqs.resetValues();
        _reqs.loadFromJson(j);
    }

    void Quest::_loadNodesFromJson(nlohmann::json& j)
    {
        for(auto i = j.begin(); i != j.end(); ++i)
        {
            std::string name = i.key();
            auto found = _nodes.find(name);
            if(found == _nodes.end())
            {
                assert(false);
            }
            auto& node = found->second;
            json jNode = i.value();
            node->loadSaveFromJson(jNode);
        }
    }

    void Quest::_resetNodesReqs()
    {
        
    }
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

    void Quest::initScript(Script& script) 
    {
        auto object = script.newClass<Quest>("Quest", "QuestLog");

        // Main
        object.set("getNode", &Quest::getNode);
        //object.set("setTitle", &Quest::setTitle);
        object.set("getRoot", &Quest::getRoot);
        object.set("getReqs", &Quest::getReqs);
        object.set("setNameIndex", &Quest::setNameIndex);
        object.set("nextStep", &Quest::nextStep);

        object.init();
    }
}