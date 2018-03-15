#include "Dialog.hpp"

namespace rat {
    Dialog::Dialog() :
    _dialogGUI(getModule<GUI>()) {
        LOG_INFO(this, "Module Dialog constructed")
        auto& gui = getModule<GUI>();
        
        _initScript();
        getModule<Script>().scriptFile("data/_dialog.lua");
    }


    Dialog::~Dialog() {
        LOG_INFO(this, "Module Dialog destructed")
        for(auto& it : _dialogs)
            delete it.second;
    }

    void Dialog::_initScript() {
        Script& script = getModule<Script>();
        auto module = script.newModule("Dialog");

        SCRIPT_SET_MODULE(Dialog, load);
        module.set("GUI", &_dialogGUI);


        script.initClasses<DialogManager, Options, DialogGUI>();
    }



    void Dialog::update(float deltaTime) {
        for(auto& it : _dialogs)
            it.second->update(deltaTime);
        if(getModule<Input>().getManager().isReleased(Keyboard::Space))
            for(auto& it : _dialogs)
                it.second->skip();
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