#include "DialogManager.hpp"
#include "TextStruct.hpp"
#include <iostream>

namespace rat {
    void DialogManager::update() {
        _soundManager.update();
    }

    void DialogManager::load(const std::string& path) {
        _textManager.load(path+".dgl");
        _soundManager.newSound(path);
        _soundManager.changeSound(path);
    }

    const std::string& DialogManager::play(size_t id) {
        _textManager.set(id);
        auto* strct = _textManager.getStruct();
        _soundManager.addOffset((float)strct->getVoiceStart(), (float)strct->getVoiceLength());
        _soundManager.setPlayingOffset((float)strct->getVoiceStart());
        _soundManager.play();
        return strct->getText();
    }
}