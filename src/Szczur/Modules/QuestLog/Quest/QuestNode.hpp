#pragma once

#include <vector>
#include <functional>
#include <memory>
#include <string>
#include <set>

#include "Requirements.hpp"

namespace rat
{
    class Quest; class QuestTitle;
    class QuestNode : public Requirements
    {
        using Function_t = std::function<void()>;
        using Node_t = std::unique_ptr<QuestNode>;
    public:
        enum class Type { Starting, Step, Brancher};
        enum class State { Inactivated, Activated, Chosen, Blocked, Finished};

        QuestNode(Quest* parentQuest, Type type, const std::string& name = "");
        QuestNode(Quest* parentQuest, const std::string& name = "");
        
        void setParent(QuestNode* parent);

        QuestNode* addStep(const std::string& name = "");
        QuestNode* addBrancher(const std::string& name = "");
        QuestNode* addSubNode(const std::string& name = "");
        QuestNode* addNode(QuestNode* node);

        void start();
        void finish();

        void nextStep(size_t nextNodeIndex = 0);

        void setActivateCallback(Function_t onActivate);
        void setBlockedCallback(Function_t onBlocked);
        void setFinishedCallback(Function_t onFinished);

        void setTitle(const std::string& title);
        void setName(const std::string& name);

        virtual void advanceCounter(const std::string& name, int value = 1) override;
        virtual void suitReq(const std::string& name) override;        


        Function_t _onActivate = [](){};
        Function_t _onBlocked = [](){};
        Function_t _onFinished = [](){};
    private:
        void _blockNotChosenNodes();
        void _invokeParentToBlockNeighbours();

        Type _type;
        State _state{ State::Inactivated };

        Quest* _parentQuest{nullptr};

        QuestNode* _startingNode{nullptr};
        QuestNode* _parentNode{nullptr};
        QuestNode* _currentChildNode{nullptr};

        std::vector<QuestNode*> _nextNodes;
        std::vector<QuestNode*> _requirmentNodes;
        void _fillChildWithLove(QuestNode* child);

        void _activateNextNode(size_t nextNodeIndex = 0);

        bool _canBeFinished() const;

        void _block();
        void _activate(QuestNode* parent = nullptr);
        bool _isFinished() const;

        std::set<QuestNode*> _getStartingNodes();

        size_t _localIndex = 0;
        size_t _level = 0;

        void _onActivateGUISet();
        void _onFinishedGUISet();
        QuestTitle* _titleGUI{nullptr};


        std::string _title{""};
        std::string _name{""};
    };
}