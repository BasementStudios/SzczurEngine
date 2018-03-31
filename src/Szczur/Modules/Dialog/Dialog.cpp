#include "Dialog.hpp"

namespace rat {
    Dialog::Dialog() :
    _dialogGUI(getModule<GUI>()) {
        LOG_INFO(this, "Module Dialog constructed")
        auto& gui = getModule<GUI>();
        _initScript();
        _dialogGUI.setKillerCallback([this](){
            unload();
        });
    }


    Dialog::~Dialog() {
        LOG_INFO(this, "Module Dialog destructed")
        delete _dialogManager;
    }

    void Dialog::_initScript() {
        Script& script = getModule<Script>();
        auto module = script.newModule("Dialog");

		module.set("Dialog", &Dialog::load);
        module.set("GUI", &_dialogGUI);


        script.initClasses<DialogManager, Options, DialogGUI>();
    }



    void Dialog::update(float deltaTime) {
        if(_dialogManager) {
            if(_dialogManager->update(deltaTime))
                unload();
            if(getModule<Input>().getManager().isReleased(Keyboard::Space))
                _dialogManager->skip();
        }
    }

    DialogManager* Dialog::load(const std::string& path) {
        _dialogManager = new DialogManager(path, _dialogGUI);
        _dialogGUI.show();
        return _dialogManager;
    }

    void Dialog::unload() {
        delete _dialogManager;
        _dialogManager = nullptr;
        _dialogGUI.hide();
    }
}