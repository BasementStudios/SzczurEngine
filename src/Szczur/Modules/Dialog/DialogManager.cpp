#include "DialogManager.hpp"
#include "TextStruct.hpp"
#include <iostream>
#include <sol2/sol.hpp>

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
        object.set("bindCharacter", &DialogManager::bindCharacter);
        object.set(
            "newOptions", 
            [](DialogManager& owner) {
                Options* options = new Options;
                owner.addOptions(options);
                return options;
            }
        );

        //
        object.init();
    }

    void DialogManager::bindCharacter(const std::string& character, const std::string& path) {
        sf::Texture* texture = new sf::Texture;
        if(texture->loadFromFile(path)) {
            Hash32_t key = fnv1a_32(character.begin(), character.end());
            if(auto it = _charactersBinds.find(key); it != _charactersBinds.end())
                delete texture;
            else
                _charactersBinds[key] = texture;
        }
        else {
            delete texture;
            LOG_ERROR(this, " cannot load: ", path)
        }
    }

	bool DialogManager::isDialogPlaying()
	{
		return _isDialogPlaying;
	}

    void DialogManager::skip() {
        _soundManager.skip();
    }

    bool DialogManager::update(float deltaTime) {
        if(_clearButtons) {
            _clearButtons = false;
            _dialogGUI.clear();
        }
        if(!_paused) {
            auto state = _soundManager.update();
            if(state == sf::SoundSource::Status::Paused) {
                //if(!_nextMinor()) { //Finished every minor in this id
				if (_destroyed)
				{
					_isDialogPlaying = false;
					return true;
				}
                _paused = true;
                for(auto& it : _options) {
                    if(it->checkIfRunsWith(_currentMajor, _currentMinor)) {
                        _dialogGUI.interpretOptions(_textManager, *it, [this](size_t major, size_t minor, bool finishing){
                            _currentMajor = major;
                            _currentMinor = minor;
                            this->_destroyed = finishing;
                            return play();
                        });
                        break;
                    }
                }
            }
        }

        return false;
    }

    void DialogManager::load(const std::string& path) {
        _textManager.load(path+".dlg");
        _soundManager.load(path);
    }

    void DialogManager::startWith(size_t major, size_t minor) {
        _currentMajor = major;
        _currentMinor = minor;
    }
    
    void DialogManager::play() {
        _paused = false;
		_isDialogPlaying = true;
        return play(_currentMajor, _currentMinor);
    }

    void DialogManager::play(size_t major, size_t minor) {
        _textManager.set(major, minor);
        _changeStruct(_textManager.getStruct());
        _clearButtons = true;
        _dialogGUI.show();
    }


    /*bool DialogManager::_nextMinor() {
        _textManager.nextMinor();
        if(_textManager.isMinorFinished())
            return false;
        _changeStruct(_textManager.getStruct());
        return true;
    }*/

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
                    _dialogGUI.setCharacterName(it->second.first);
                    if(
                        auto result = _charactersBinds.find(
                            fnv1a_32(
                                it->second.first.begin(), 
                                it->second.first.end()
                            )
                        );
                        result != _charactersBinds.end()
                    ) {
                        _dialogGUI.setCharacterTexture( result->second );
                    }
                }
            );
        });

        _soundManager.play();
        
    }
}
