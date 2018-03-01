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
                                /*
                                Problemem jest czyszczenie kontenera widgetów, w czasie 
                                kiedy wątek obsluguje jeden widget i wywołuje callback 
                                onRelease. Callback onRelease czyści
                                */
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

    void DialogManager::play(size_t id) {
        _textManager.set(id);
        auto* strct = _textManager.getStruct();
        _soundManager.addOffset((float)strct->getVoiceStart(), (float)strct->getVoiceEnd());
        _soundManager.setPlayingOffset((float)strct->getVoiceStart());
        _soundManager.play();
        _dialogGUI.setText( strct->getText() );
        _clearButtons = true;
    }

    void DialogManager::play() {
        _paused = false;
        return play(_current);
    }

    void DialogManager::startWith(size_t id) {
        _current = id;
    }

    bool DialogManager::_nextMinor() {
        _textManager.nextMinor();
        if(_textManager.isMinorFinished())
            return false;
        auto* strct = _textManager.getStruct();
        _soundManager.addOffset((float)strct->getVoiceStart(), (float)strct->getVoiceEnd());
        _soundManager.setPlayingOffset((float)strct->getVoiceStart());
        _soundManager.play();
        return true;
    }
}