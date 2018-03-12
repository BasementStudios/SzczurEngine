#include "DialogManager.hpp"
#include "TextStruct.hpp"
#include <iostream>

namespace rat {
    DialogManager::DialogManager(const std::string& path, DialogGUI& dialogGUI) :
    _paused(true),
    _clearButtons(false),
    _dialogGUI(dialogGUI) {
        load(path);
    }

    DialogManager::~DialogManager() {
        for(auto it : _options)
            delete it;
    }

    void DialogManager::initScript(Script& script) {
        auto object = script.newClass<DialogManager>("DialogManager", "Dialog");

        object.set("startWith", &DialogManager::startWith);
        object.set("play", sol::resolve<void()>(&DialogManager::play));
        object.set(
            "newOptions", 
            [](DialogManager& owner, sol::variadic_args va) {
                Options* options = new Options;
                for(auto v : va)
                    options->addRunners<Options::Key_t>(v);

                owner.addOptions(options);
                return options;
            }
        );

        //
        object.init();
    }

    void DialogManager::skip() {
        _soundManager.skip();
    }

    void DialogManager::update(float deltaTime) {
        if(_clearButtons) {
            _clearButtons = false;
            _dialogGUI.clear();
        }
        if(!_paused) {
            auto state = _soundManager.update();
            if(state == sf::SoundSource::Status::Paused) {
                if(!_nextMinor()) { //Finished every minor in this id
                    _paused = true;
                    for(auto& it : _options) {
                        if(it->checkIfRunsWith(_current)) {
                            _dialogGUI.interpretOptions(*it, [this](size_t id){
                                _current = id;
                                return play();
                            });
                            break;
                        }
                    }
                }
            }
        }
        else {

        }
    }

    void DialogManager::load(const std::string& path) {
        _textManager.load(path+".dlg");
        _soundManager.load(path);
    }

    void DialogManager::startWith(size_t id) {
        _current = id;
    }
    
    void DialogManager::play() {
        _paused = false;
        return play(_current);
    }

    void DialogManager::play(size_t id) {
        _textManager.set(id);
        _changeStruct(_textManager.getStruct());
        _clearButtons = true;
    }


    bool DialogManager::_nextMinor() {
        _textManager.nextMinor();
        if(_textManager.isMinorFinished())
            return false;
        _changeStruct(_textManager.getStruct());
        return true;
    }

    void DialogManager::_changeStruct(TextStruct* strct) {
        _soundManager.eraseCallbacks();
        _soundManager.addOffset(
            strct->getVoiceStart(), 
            strct->getVoiceEnd()
        );
        _soundManager.setPlayingOffset(strct->getVoiceStart());
        strct->forEach([this](TextStruct::Texts_t::iterator it){
            _soundManager.addCallback( 
                it->first,
                [this, it](){
                    _dialogGUI.setText(it->second.second);
                }
            );
        });

        _soundManager.play();
        
    }
}