#pragma once

#include <vector>
#include <functional>
#include <memory>
#include <string>
#include <set>

#include <sol.hpp>
#include <Json/json.hpp>

#include "Szczur/Modules/QuestLog/GUI/QuestInfoBar/QuestTitle.hpp"

#include "Requirements.hpp"

namespace rat
{
    class Quest; class QuestTitle;
    class QuestNode
    {
        using Function_t = sol::function;
        using Node_t = std::unique_ptr<QuestNode>;
    public:
        enum class Type { Starting, Step, Brancher};
        enum class State { Inactivated, Activated, Chosen, Blocked, Finished};

        QuestNode(Quest* parentQuest, Type type, const std::string& name = "");
        QuestNode(Quest* parentQuest, const std::string& name = "");

        const std::string& getName() const;
        
        void setParent(QuestNode* parent);

        QuestNode* addStep(const std::string& name);
        QuestNode* addBrancher(const std::string& name );
        QuestNode* addSubNode(const std::string& name);
        QuestNode* addNode(QuestNode* node);

        void start();
        void finish();

        void nextStep(size_t nextNodeIndex = 0);

        void setActivateCallback(Function_t onActivate);
        void setBlockedCallback(Function_t onBlocked);
        void setFinishedCallback(Function_t onFinished);

        void setTitle(const std::string& title);
        void setName(const std::string& name);

        nlohmann::json getJson() const;
        void loadFromJson(nlohmann::json& j);
        void resume();

        void reset();

        Requirements& getReqs();

        Function_t _onActivate;
        Function_t _onBlocked;
        Function_t _onFinished;

        static void initScript(Script& script);
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

        void _onActivateGUISet();
        void _onFinishedGUISet();
        QuestTitle* _titleGUI{nullptr};


        std::string _title{"None"};
        std::string _name{""};

        nlohmann::json _getRequirmentNodesJson() const;

        void _loadRequirmentNodesFromJson(nlohmann::json& j);

        Requirements _reqs;

    };
}