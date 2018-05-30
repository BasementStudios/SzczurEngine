#include "QuestNode.hpp"

#include "Quest.hpp"

#include "Szczur/Utility/Logger.hpp"

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
        _onActivate();
    }

    void QuestNode::nextStep(size_t nextNodeIndex)
    {
        if(!isFullySuited()) return;
        if(_isFinished() || _state == State::Blocked) return;
        if(!_canBeFinished()) return;

        if(_type == Type::Step)
        {
            _state = State::Finished;
            _onFinished();
            _onFinishedGUISet();
            //_invokeParentToBlockNeighbours();
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
            _onFinished();
            _onFinishedGUISet();

            if(_parentNode)
            {
                _parentNode->nextStep();
            }
            else
            {
                std::cout << "Before\n";
                _parentQuest->finish();
                std::cout << "After\n";
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
        std::cout << nextNode->_title << "\n";
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
            _parentNode = this;
        }
    }

    void QuestNode::_activate(QuestNode* parent)
    {
        if(parent) _parentNode = parent;

        _state = State::Activated;

        if(_type != Type::Starting)
        {
            _startingNode->_currentChildNode = this;
        }

        _onActivateGUISet();
        _onActivate();

        size_t index = 0;
        for(auto* req : _requirmentNodes)
        {
            req->_activate(this);
        }
    }

    void QuestNode::advanceCounter(const std::string& name, int value)
    {
        Requirements::advanceCounter(name, value);
        if(isFullySuited()) nextStep();
    }

    void QuestNode::suitReq(const std::string& name)
    {
        Requirements::suitReq(name);
        if(isFullySuited()) nextStep();
    }




    void QuestNode::_onActivateGUISet()
    {   
        if(_type == Type::Step)
        {
            _parentQuest->setTitle(_title);
        }
        else
        {
            _titleGUI = _parentQuest->addSubtitle(_title, false);
        }
    }
    void QuestNode::_onFinishedGUISet()
    {
        if(_type == Type::Starting)
        {
            _titleGUI->setReq(_title, true);
        }
    }

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
    
}