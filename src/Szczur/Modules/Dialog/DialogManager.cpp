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
        _soundManager.newSound(path);
        _soundManager.changeSound(path);
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
            static_cast<float>(strct->getVoiceStart()), 
            static_cast<float>(strct->getVoiceEnd())
        );
        _soundManager.setPlayingOffset((float)strct->getVoiceStart());
        strct->forEach([this](TextStruct::Texts_t::iterator it){
            _soundManager.setCallback( 
                static_cast<float>(it->first),
                [this, it](){
                    _dialogGUI.setText(it->second.second);
                }
            );
        });

        _soundManager.play();
        
    }
}