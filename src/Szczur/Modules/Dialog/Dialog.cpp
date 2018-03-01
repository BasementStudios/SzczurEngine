#include "Dialog.hpp"

namespace rat {
    Dialog::~Dialog() {
        for(auto& it : _dialogs)
            delete it.second;
    }

    void Dialog::update(float deltaTime) {
        for(auto& it : _dialogs)
            it.second->update(deltaTime);
    }

    DialogManager* Dialog::load(const std::string& path) {
        Key_t key = fnv1a_32(path.begin(), path.end());
        if(auto it = _dialogs.find(key); it == _dialogs.end() ) {
            DialogManager* result = new DialogManager(path, _dialogGUI);
            _dialogs.insert_or_assign(key, result);
            return result;
        }
        return nullptr;
    }

    bool Dialog::unload(const std::string& path) {
        if(auto it = _dialogs.find(fnv1a_32(path.begin(), path.end())); it != _dialogs.end() ) {
            delete it->second;
            _dialogs.erase(it);
            return true;
        }
        return false;
    }
}