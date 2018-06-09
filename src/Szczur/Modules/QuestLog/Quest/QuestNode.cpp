#include "QuestNode.hpp"

#include "Quest.hpp"

#include "Szczur/Utility/Logger.hpp"

using namespace nlohmann;

namespace rat
{
    using Node_t = std::unique_ptr<QuestNode>;
    QuestNode::QuestNode(Quest* parentQuest, const std::string& name) 
    :
    QuestNode(parentQuest, Type::Starting, name)
    {}
    QuestNode::QuestNode(Quest* parentQuest, Type type, const std::string& name) 
    :
    _parentQuest(parentQuest),
    _type(type)
    {
        _parentQuest->addNode(name, std::move(std::unique_ptr<QuestNode>(this)));
    }

    QuestNode* QuestNode::addStep(const std::string& name)
    {
        auto step = new QuestNode(_parentQuest, Type::Step, name);

        return addNode(step);
    }
    QuestNode* QuestNode::addSubNode(const std::string& name)
    {
        auto* subNode = new QuestNode(_parentQuest, Type::Starting, name);

        return addNode(subNode);
    }
    QuestNode* QuestNode::addBrancher(const std::string& name)
    {
        auto* brancher = new QuestNode(_parentQuest, Type::Brancher, name);

        return addNode(brancher);
    }

    QuestNode* QuestNode::addNode(QuestNode* node)
    {

        if(node->_type == Type::Starting)
        {
            _requirmentNodes.emplace_back(node);
        }
        else if(_type == Type::Brancher) 
        {
            _nextNodes.emplace_back(node);
        }
        else
        {
            if(_nextNodes.size() == 0) _nextNodes.emplace_back(node);
            else _nextNodes.back() = node;
        } 

        return node;
    }
    

    void QuestNode::start()
    {
        if(_isFinished()) return;
        if(_state == State::Blocked) return;
        _state = State::Chosen;
        if(_onActivate.valid())
        {
            _onActivate();
        }
    }

    void QuestNode::nextStep(size_t nextNodeIndex)
    {
        if(!_reqs.isFullySuited()) return;
        if(_isFinished() || _state == State::Blocked) return;
        if(!_canBeFinished()) return;

        if(_type == Type::Step)
        {
            _state = State::Finished;
            if(_onFinished.valid())
            {
                _onFinished();
            }
            //_onFinishedGUISet();
        }
        if(_nextNodes.size() == 0)
        {
            if(_type == Type::Step)
            {
                _startingNode->finish();
            }
            else
            {
                finish();
            }
        }
        else
        {
            _activateNextNode(nextNodeIndex);
        }
    }
    
    void QuestNode::finish()
    {
        if(_isFinished()) return;
        if(!_canBeFinished()) return;        
        if(_type == Type::Starting)
        {
            _state = State::Finished;
            if(_onFinished.valid())
            {
                _onFinished();
            }
           // _onFinishedGUISet();

            if(_parentNode)
            {
                _parentNode->nextStep();
            }
            else
            {
                _parentQuest->finish();
            }
        }
        else
        {
            _startingNode->finish();
        }
    }
    bool QuestNode::_canBeFinished() const
    {
        for(auto& reqNode : _requirmentNodes)
        {
            if(!reqNode->_isFinished()) return false;
        }

        return true;
    }
    bool QuestNode::_isFinished() const{
        return _state == State::Finished;
    }

    void QuestNode::_activateNextNode(size_t nextNodeIndex)
    {
        if(_nextNodes.size() <= nextNodeIndex) nextNodeIndex = 0;

        for(size_t i = 0; i < _nextNodes.size(); i++)
        {
            if(i == nextNodeIndex) break;
            _nextNodes[i]->_block();
        }

        auto* nextNode = _nextNodes[nextNodeIndex];

        _fillChildWithLove(nextNode);
        nextNode->_activate();
    }
    void QuestNode::_fillChildWithLove(QuestNode* child)
    {
        if(_type == Type::Starting)
        {
            child->_startingNode = this;
        }
        else 
        {
            child->_startingNode = _startingNode;
        }

        if(child->_type == Type::Starting)
        {
            child->_parentNode = this;
        }
    }

    void QuestNode::_activate(QuestNode* parent)
    {
        if(parent) _parentNode = parent;


        if(_type != Type::Starting)
        {
            _startingNode->_currentChildNode = this;
        }

        //_onActivateGUISet();
        if(_state == State::Finished)
        {
            if(_onFinished.valid())
            {
                _onFinished();
            }
        }
        else
        {
            _state = State::Activated;
            if(_onActivate.valid())
            {
                _onActivate();
            }
        }

        for(auto* req : _requirmentNodes)
        {
            req->_activate(this);
        }
    }
/*
    void QuestNode::_onActivateGUISet()
    {   
        TitleInfo info;

        if(_type == Type::Starting)
        {
            if(_parentNode == nullptr)
            {
                info.makeTitle(_title + "kek");
                _parentQuest->setTitle(info);
            }
            else
            {
                info.makeReq(_title + "kek", _state == State::Finished);
                _titleGUI = _parentQuest->addSubtitle(info);
            }
        }
        else
        {
            if(_startingNode->_parentNode == nullptr)
            {
                info.makeTitle(_title + "kek");
                _parentQuest->setTitle(info);
            }
            else
            {
                info.makeReq(_title + "kek", _state == State::Finished);
                _titleGUI = _parentQuest->addSubtitle(info);
            }
        }
        LOG_INFO("_onActivateGUISet ", _name, " has beed activated", _title);
    }
    /*
    void QuestNode::_onFinishedGUISet()
    {
        if(_type == Type::Starting)
        {
            TitleInfo info;
            info.makeReq(_title, true);
            if(_titleGUI)
                _titleGUI->setTitle(info);
        }
    }
*/
    void QuestNode::setActivateCallback(Function_t onActivate)
    {
        _onActivate = onActivate;
    }
    void QuestNode::setBlockedCallback(Function_t onBlocked)
    {
        _onBlocked = onBlocked;        
    }
    void QuestNode::setFinishedCallback(Function_t onFinished)
    {
        _onFinished = onFinished;
    }

    void QuestNode::_block()
    {
        _state = State::Blocked;
        _onBlocked();
    }
    
    void QuestNode::setTitle(const std::string& title)
    {
        _title = title;
    }
    void QuestNode::setName(const std::string& name)
    {
        _name = name;
    }

   
    void QuestNode::resume()
    {
        if(_currentChildNode)
        {
            _currentChildNode->resume();
        }
        else
        {
            if(_state == State::Finished)
            {
                if(_onFinished.valid())
                {
                    _onFinished();
                }
            }
            else
            {
                if(_onActivate.valid())
                {
                    _onActivate();
                }
            }
            for(auto req : _requirmentNodes)
            {
                req->resume();
            }
        }
    }

    const std::string& QuestNode::getName() const
    {
        return _name;
    }
    

    json QuestNode::getJson() const
    {
        json j;

        j["state"] = static_cast<int>(_state);

        bool hasCurrent = bool(_currentChildNode);
        j["hasCurrent"] = hasCurrent;
        if(hasCurrent)
        {
            j["current"] = _currentChildNode->getJson();
            j["currentsName"] = _currentChildNode->getName();
        }

        j["requeirmentNodes"] = _getRequirmentNodesJson();

        j["reqs"] = _reqs.getJson();

        return j;
    }

    json QuestNode::_getRequirmentNodesJson() const
    {
        json j;
        for(auto req : _requirmentNodes)
        {
            j.emplace_back(req->getJson());
        }
        return j;
    }

    void QuestNode::loadFromJson(nlohmann::json& j)
    {
        _state = static_cast<State>(j["state"]);

        if(j["hasCurrent"])
        {
            auto& jCur = j["current"];
            std::string currentName = j["currentsName"];

            _currentChildNode = _parentQuest->getNode(currentName);
            _currentChildNode->loadFromJson(jCur);
            _fillChildWithLove(_currentChildNode);
        }


        _loadRequirmentNodesFromJson(j["requeirmentNodes"]);

        _reqs.loadFromJson(j["reqs"]);
    }

    void QuestNode::_loadRequirmentNodesFromJson(nlohmann::json& j)
    {
        LOG_ERROR_IF(j.size() != _requirmentNodes.size(), "Node \"", _name, "\" cannot load requirment nodes properly...");
        for(int i = 0; i < j.size(); i++)
        {
            auto req = _requirmentNodes[i];
            req->loadFromJson(j[i]);
            _fillChildWithLove(req);
            req->_activate(this);
        }
    }

    void QuestNode::reset()
    {
        _state = State::Inactivated;
        _startingNode = nullptr;
        _reqs.resetValues();
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
        object.set("addBrancher", &QuestNode::addBrancher);
        object.set("addSubNode", &QuestNode::addSubNode);
        object.set("addNode", &QuestNode::addNode);
        object.set("setTitle", &QuestNode::setTitle);
        object.set("nextStep", &QuestNode::nextStep);
        object.set("getReqs", &QuestNode::getReqs);

        // Callbacks
        object.set("onActivate", &QuestNode::_onActivate);
        object.set("onBlock", &QuestNode::_onBlocked);
        object.set("onFinish", &QuestNode::_onFinished);

        object.init();
    }
}