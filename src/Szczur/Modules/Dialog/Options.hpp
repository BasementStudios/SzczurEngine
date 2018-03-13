#pragma once

#include <functional>
#include <vector>

#include "Szczur/Modules/Script/Script.hpp"

#include <boost/container/flat_set.hpp>

namespace rat {
    class Options {
    public:
        struct Option;

        using Key_t = size_t;
        using Runners_t = boost::container::flat_set<Key_t>;
        template<typename R>
        using Callback_t = std::function<R()>;
        using Condition_t = Callback_t<bool>;
        using AfterAction_t = Callback_t<void>;
        using Options_t = std::vector<Option*>;

        struct Option {
            Condition_t condition;
            Key_t target;
            AfterAction_t afterAction;
        };

        Options();
        ~Options();

        static void initScript(Script& script);

        template<typename... Ts>
        void setRunners(Ts... runners) {
            _runners.clear();
            (_runners.insert(runners), ...);
        }

        template<typename... Ts>
        void addRunners(Ts... runners) {
            (_runners.insert(runners), ...);
        }

        void addOption(
            Condition_t condition, 
            Key_t target, 
            AfterAction_t afterAction
        );

        void forEach(std::function<void(Option*)> func);

        bool checkIfRunsWith(Key_t id) const;
    private:
        Options_t _options;
        Runners_t _runners;
    };
}