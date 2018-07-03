#include "QuestNode.hpp"

#include <algorithm>

#include "Quest.hpp"

using namespace nlohmann;

namespace rat
{
    QuestNode::QuestNode(Quest* baseQuest, Type type, const std::string& name) 
    :
    _baseQuest(baseQuest),
    _type(type)
    {
        _baseQuest->addNode(name, std::move(std::unique_ptr<QuestNode>(this)));
    }

    QuestNode::QuestNode(Quest* baseQuest, const std::string& name) 
    :
    QuestNode(baseQuest, Type::Base, name)
    {}

    bool QuestNode::isInProgress() const
    {
        return _state == State::InProgress;
    }

    QuestNode* QuestNode::addStep(const std::string& name)
    {
        auto* step = new QuestNode(_baseQuest, Type::Step, name);
        return addNode(step);
    }

    QuestNode* QuestNode::addSubNode(const std::string& name)
    {
        auto* req = new QuestNode(_baseQuest, Type::Base, name);
        return addNode(req);
    }

    QuestNode* QuestNode::addNode(QuestNode* node)
    {
        const auto nodeName = node->getName();
        if(node->_type == Type::Step)
        {
            _nextNodesNames.emplace(nodeName);
            if(_type == Type::Base)
            {
                node->_setBaseNode(getName());
            }
            else
            {
                assert(_hasBaseNode);
                node->_setBaseNode(_baseNodeName);
            }
        }
        else
        {
            _reqNodes.emplace(nodeName, false);
            node->_setBrancherNode(getName());
        }

        return node;
    }

    void QuestNode::_setBaseNode(const std::string& baseNodeName)
    {
        _hasBaseNode = true;
        _baseNodeName = baseNodeName;
    }
    void QuestNode::_setBrancherNode(const std::string& brancherNodeName)
    {
        _hasBrancher = true;
        _brancherNodeName = brancherNodeName;
    }

    void QuestNode::activate()
    {
        _state = State::InProgress;
        if(onActivate.valid()) onActivate();
        if(_hasTitle) _baseQuest->addTitle(_titleIndex);
        if(_hasDescription) _baseQuest->addDescription(_descriptionIndex);
    }
    void QuestNode::resume()
    {
        
    }
    void QuestNode::block()
    {
        _state = State::Blocked;
        if(onBlocked.valid()) onBlocked();
    }

    void QuestNode::nextStep(const std::string& name)
    {
        if(onFinished.valid()) onFinished();

        if(_nextNodesNames.size() == 0)
        {
            if(_hasBrancher)
            {
                auto* brancherNode = _baseQuest->getNode(_brancherNodeName);
                brancherNode->_suitReqNode(getName());
            }
            else if(_hasBaseNode)
            {
                auto* baseNode = _baseQuest->getNode(_baseNodeName);
                baseNode->_finish();
            }
            else
            {
                std::cout << "Node \"" << _name << "\" finishes quest \"" << _baseQuest->getName() << "\"...\n\n";
                _baseQuest->finish();
            }
        }
        else
        {
            std::string nextNodeName;
            if(name == "")
            {
                nextNodeName = *(_nextNodesNames.begin());
            }
            else
            {
                nextNodeName = name;
            }
            auto* nextNode = _baseQuest->getNode(nextNodeName);
            
            _blockNextNodesWithout(nextNodeName);
            
            nextNode->activate();
        }
    }

    void QuestNode::_blockNextNodesWithout(const std::string& name)
    {
        for(const auto& nodeName : _nextNodesNames)
        {
            if(nodeName == name) continue;

            auto* node = _baseQuest->getNode(nodeName);
            node->block();
        }
    }
    

    const std::string& QuestNode::getName() const
    {
        return _name;
    }
    void QuestNode::setName(const std::string& name)
    {
        _name = name;
    }

    void QuestNode::_suitReqNode(const std::string& name)
    {
        if(auto node = _reqNodes.find(name); node == _reqNodes.end())
        {
            LOG_ERROR("Cannot find req node: \"", name, "\" to suit");
            assert(false);
        }
        else
        {
            node->second = true;
            _tryNextStep();
        }
    }
    void QuestNode::_tryNextStep()
    {
        if(_areAllReqNodesSuited())
        {
            nextStep();
        }
    }
    bool QuestNode::_areAllReqNodesSuited() const
    {
        return std::all_of(_reqNodes.begin(), _reqNodes.end(), [](const auto& req){
            return req.second;
        });
    }
    void QuestNode::_finish()
    {
        auto* quest = _baseQuest;
        _state = State::Finished;

        if(_type == Type::Base)
        {
            if(_hasBrancher)
            {
                auto* brancher = quest->getNode(_brancherNodeName);
                brancher->_suitReqNode(getName());
            }
            else
            {
                quest->finish();
            }
        }
        else if(_hasBaseNode)
        {
            auto* base = quest->getNode(_baseNodeName);
            base->_finish();
        }
    }

    void QuestNode::addTitle(int index)
    {
        _titleIndex = index;
        _hasTitle = true;
    }
    void QuestNode::addDescription(int index)
    {
        _descriptionIndex = index;
        _hasDescription = true;
    }


    json QuestNode::getSaveJson() const
    {
        json j;

        j["state"] = static_cast<int>(_state);
        if(_hasBaseNode) j["baseNode"] = _baseNodeName;
        if(_hasBrancher) j["brancherNode"] = _brancherNodeName;

        j["reqs"] = _reqs.getJson();

        //j["reqNodes"] = _getReqNodesJson();
        //j["nextNodes"] = _getNextNodesJson();

        return j;
    }

    nlohmann::json QuestNode::_getReqNodesJson() const
    {
        return json(_reqNodes);
    }
    nlohmann::json QuestNode::_getNextNodesJson() const
    {
        return json(_nextNodesNames);
    }

    void QuestNode::loadSaveFromJson(nlohmann::json& j)
    {
        _state = static_cast<State>(j["state"]);

        if(j.find("baseNode") != j.end()) 
        {
            _baseNodeName = j["baseNode"];
            _hasBaseNode = true;
        }
        else
        {
            _baseNodeName = {};
            _hasBaseNode = false;
        }

        if(j.find("brancherNode") != j.end()) 
        {
            _brancherNodeName = j["brancherNode"];
            _hasBrancher = true;
        }
        else
        {
            _brancherNodeName = {};
            _hasBrancher = false;
        }

        _reqs.loadFromJson(j["reqs"]);
    }

    Requirements& QuestNode::getReqs()
    {
        return _reqs;
    }

    void QuestNode::initScript(Script& script) 
    {
        auto object = script.newClass<QuestNode>("QuestNode", "QuestLog");

        // Main
        object.set("getName", &QuestNode::getName);
        object.set("addStep", &QuestNode::addStep);
        object.set("addSubnode", &QuestNode::addSubNode);
        object.set("addNode", &QuestNode::addNode);
        object.set("nextStep", &QuestNode::nextStep);
        object.set("getReqs", &QuestNode::getReqs);

        object.set("addDescription", &QuestNode::addDescription);
        object.set("addTitle", &QuestNode::addTitle);


        // Callbacks
        object.set("onActivate", &QuestNode::onActivate);
        object.set("onBlock", &QuestNode::onBlocked);
        object.set("onFinish", &QuestNode::onFinished);

        object.init();
    }
}