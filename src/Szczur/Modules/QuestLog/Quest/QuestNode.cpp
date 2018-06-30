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


    QuestNode* QuestNode::addStep(const std::string& name)
    {
        auto* step = new QuestNode(_baseQuest, name);
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
        }
        else
        {
            _reqNodes.emplace(nodeName, false);
        }

        return node;
    }

    void QuestNode::activate()
    {
        _state = State::InProgress;
        if(onActivate.valid()) onActivate();
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
}