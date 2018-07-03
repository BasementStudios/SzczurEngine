#pragma once

#include <vector>
#include <functional>
#include <memory>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include <sol.hpp>
#include <Json/json.hpp>

#include "Requirements.hpp"

namespace rat
{
    class Quest; class QuestTitle;
    class QuestNode
    {
        using Function_t = sol::function;
        using Node_t = std::unique_ptr<QuestNode>;
    public:
        enum class Type { Base, Step, Brancher};
        enum class State { Inactivated, InProgress, Blocked, Finished};

        QuestNode(Quest* baseQuest, Type type, const std::string& name = "");
        QuestNode(Quest* baseQuest, const std::string& name = "");

        static void initScript(Script& script);

    public:
        QuestNode* addStep(const std::string& name);
        QuestNode* addNode(QuestNode* node);
        QuestNode* addSubNode(const std::string& name);

        void activate();
        void resume();
        void block();

        void nextStep(const std::string& name = "");

        void setName(const std::string& name);
        const std::string& getName() const;

        void addTitle(int index);
        void addDescription(int index);

        Function_t onActivate;
        Function_t onBlocked;
        Function_t onFinished;

        bool isInProgress() const;

        nlohmann::json getSaveJson() const;

        void loadSaveFromJson(nlohmann::json& j);

        Requirements& getReqs();

    private:
        State _state{State::Inactivated};
        Type _type;

        std::string _name;

        void _finish();

        void _suitReqNode(const std::string& name);
        void _tryNextStep();
        bool _areAllReqNodesSuited() const;

        void _blockNextNodesWithout(const std::string& name);

        Requirements _reqs;

        std::unordered_set<std::string> _nextNodesNames;
        std::unordered_map<std::string, bool> _reqNodes;

        std::string _baseNodeName;
        bool _hasBaseNode{false};
        void _setBaseNode(const std::string& baseNodeName);

        std::string _brancherNodeName;
        bool _hasBrancher{false};
        void _setBrancherNode(const std::string& brancherNodeName);

        bool _hasOnActivatePath{false};
        bool _hasOnBlockPath{false};
        bool _hasOnFinishPath{false};

        Quest* _baseQuest{nullptr};

        bool _hasTitle{false};
        int _titleIndex{-1};

        bool _hasDescription{false};
        int _descriptionIndex{-1};


        nlohmann::json _getReqNodesJson() const;
        nlohmann::json _getNextNodesJson() const;

    };
}