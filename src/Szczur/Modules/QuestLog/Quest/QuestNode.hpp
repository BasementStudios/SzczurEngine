#pragma once

#include <vector>
#include <functional>
#include <memory>
#include <string>
#include <set>

#include "Requirements.hpp"

namespace rat
{
    class Quest;
    class QuestNode : public Requirements
    {
        using Function_t = std::function<void()>;
        using Node_t = std::unique_ptr<QuestNode>;
    public:
        enum class Type { Starting, Step };
        enum class State { Inactivated, Activated, Chosen, Blocked, Finished};

        QuestNode(Quest* parentQuest, Type type, const std::string& name = "");
        QuestNode(Quest* parentQuest, const std::string& name = "");
        
        void setParent(QuestNode* parent);

        QuestNode* addStep(const std::string& name = "");
        QuestNode* addSubNode(const std::string& name = "");
        QuestNode* addNode(QuestNode* node);

        void start();
        void finish();
        void nextStep();

        void setActivateCallback(Function_t onActivate);
        void setBlockedCallback(Function_t onBlocked);
        void setFinishedCallback(Function_t onFinished);

        void setTitle(const std::string& title);

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

        std::set<QuestNode*> _startingNodes;
        std::set<QuestNode*> _parentNodes;

        std::vector<QuestNode*> _nextNodes;
        std::vector<QuestNode*> _requirmentNodes;

        void _activateNextNodes();

        bool _canBeFinished() const;

        void _block();
        void _activate();
        bool _isFinished() const;

        std::set<QuestNode*> _getStartingNodes();


        std::string _title{""};

    };
}