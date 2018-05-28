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
    
    void QuestNode::setParent(QuestNode* parent)
    {
        _parentNodes.emplace(parent);
        auto parentStartingNodes = parent->_getStartingNodes();
        for(auto* startingNode : parentStartingNodes)
        {
            _startingNodes.emplace(startingNode);
        }
    }
    std::set<QuestNode*> QuestNode::_getStartingNodes()
    {
        if(_type == Type::Starting) return {this};
        return _startingNodes;
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

    QuestNode* QuestNode::addNode(QuestNode* node)
    {
        node->setParent(this);

        if(node->_type == Type::Step) _nextNodes.emplace_back(node);
        else _requirmentNodes.emplace_back(node);

        return node;
    }
    

    void QuestNode::start()
    {
        if(_isFinished()) return;
        if(_state == State::Blocked) return;
        _state = State::Chosen;
        _onActivate();
    }

    void QuestNode::nextStep()
    {
        if(!isFullySuited()) return;
        if(_isFinished() || _state == State::Blocked) return;
        if(!_canBeFinished()) return;

        if(_type == Type::Step)
        {
            _state = State::Finished;
            _onFinished();
            _invokeParentToBlockNeighbours();
        }
        if(_nextNodes.size() == 0)
        {
            if(_type == Type::Step)
            {
                for(auto* starting : _startingNodes)
                {
                    starting->finish();
                }
            }
            else
            {
                finish();
            }
        }
        else
        {
            _activateNextNodes();
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

            if(_parentNodes.size() > 0)
            {
                for(auto* parent : _parentNodes)
                {
                    parent->nextStep();
                }
            }
            else
            {
                _parentQuest->finish();
            }
        }
        else
        {
            for(auto* starting : _startingNodes)
            {
                starting->finish();
            }
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

    void QuestNode::_activateNextNodes()
    {
        size_t i = 0;
        for(auto& nextNode : _nextNodes)
        {
            nextNode->_activate();
            std::cout << nextNode->_title << "\n";
            i++;
            if(i != _nextNodes.size() && _nextNodes.size() > 1)
                std::cout << "OR\n";
        }
        std::cout << "\n";
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

    void QuestNode::_activate()
    {
        _state = State::Activated;
        _onActivate();
        for(auto* req : _requirmentNodes)
        {
            req->_activate();
        }
    }

    void QuestNode::_invokeParentToBlockNeighbours()
    {
        for(auto* parent : _parentNodes)
        {
            parent->_blockNotChosenNodes();
        }
    }
    void QuestNode::_blockNotChosenNodes()
    {
        for(auto& nextNode : _nextNodes)
        {
            auto& state = nextNode->_state;
            if(state != State::Finished)
            {
                if(nextNode->_type != Type::Starting)
                {
                    nextNode->_block();
                }
            }
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
    
}