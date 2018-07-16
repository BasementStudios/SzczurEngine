#include "Options.hpp"
#include <iostream>
namespace rat {
    Options::Options() {

    }

    Options::~Options() {
        for(auto it : _options)
            delete it;
    }

    void Options::initScript(Script& script) {
        auto object = script.newClass<Options>("Options", "Dialog");
        
        object.set("addRunner", &Options::addRunner);

        object.setProperty(
            "add",
            []() {},
            [](Options& owner, sol::table tab) {
                if(tab["majorTarget"].valid() && tab["minorTarget"].valid()) {
                    owner.addOption(
                        ((tab["condition"].valid()) ? (tab["condition"].get<sol::function>()) : sol::function{}),
                        tab["majorTarget"],
                        tab["minorTarget"],
                        ((tab["action"].valid()) ? (tab["action"].get<sol::function>()) : sol::function{}),
                        (tab["finishing"].valid()) ? tab["finishing"] : false,
                        (tab["skip"].valid()) ? tab["skip"] : false,
						(tab["color"].valid()) ? sf::Color(tab["color"][1].get<int>(), tab["color"][2].get<int>(), tab["color"][3].get<int>(), tab["color"][4].get<int>()) : sf::Color::White,
                        (tab["iconId"].valid()) ? tab["iconId"] : -1
					);
                }
            }
        );
        

        object.init();
    }

    void Options::addRunner(size_t major, size_t minor) {
        _runners.insert({major, minor});
    }

    void Options::addOption(Condition_t condition, Key_t majorTarget, Key_t minorTarget, AfterAction_t afterAction, bool finishing, bool skip, sf::Color color, int iconId) {
        _options.push_back(new Option{condition, majorTarget, minorTarget, afterAction, finishing, skip, color, iconId});
    }

    bool Options::checkIfRunsWith(Key_t major, Key_t minor) const {
        return _runners.find({major, minor}) != _runners.end();
    }

    void Options::forEach(std::function<void(Option*)> func) {
        for(auto it : _options)
            std::invoke(func, it);
    }
}
