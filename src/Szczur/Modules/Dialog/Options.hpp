#pragma once

#include <functional>
#include <vector>

#include "Szczur/Modules/Script/Script.hpp"

#include <SFML/Graphics/Color.hpp>

#include <boost/container/flat_set.hpp>

namespace rat {
    class Options {
    public:
        struct Option;

        using Key_t = size_t;
        using Runners_t = boost::container::flat_set<std::pair<size_t, size_t>>;
        /*template<typename R>
        using Callback_t = std::function<R()>;
        using Condition_t = Callback_t<bool>;
        using AfterAction_t = Callback_t<void>;*/
        using Condition_t = sol::function;
        using AfterAction_t = sol::function;
        using Options_t = std::vector<Option*>;

        struct Option {
            Condition_t condition;
            Key_t majorTarget;
            Key_t minorTarget;
            AfterAction_t afterAction;
            bool finishing;
            bool skip;
			sf::Color color;
			int iconId;
        };

        Options();
        ~Options();

        static void initScript(Script& script);

        void addRunner(size_t major, size_t minor);

        void addOption(
            Condition_t condition, 
            Key_t majorTarget,
            Key_t minorTarget, 
            AfterAction_t afterAction,
            bool finishing,
            bool skip,
			sf::Color color,
			int iconId
        );

        void forEach(std::function<void(Option*)> func);

        bool checkIfRunsWith(Key_t major, Key_t minor) const;
    private:
        Options_t _options;
        Runners_t _runners;
    };
}