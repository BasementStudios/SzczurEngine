#include "Options.hpp"

namespace rat {
    Options::Options() {

    }

    Options::~Options() {
        for(auto it : _options)
            delete it;
    }

    void Options::addOption(const std::string& name, Callback_t condition, Key_t target, Callback_t afterAction) {
        _options.push_back(new Option{name, condition, target, afterAction});
    }

    bool Options::checkIfRunsWith(Key_t id) const {
        return _runners.find(id) != _runners.end();
    }

    void Options::forEach(std::function<void(Option*)> func) {
        for(auto it : _options)
            std::invoke(func, it);
    }
}
