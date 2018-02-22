#pragma once

#include "TextManager.hpp"
#include "SoundManager.hpp"

namespace rat {
    class DialogManager {
    public:
        void update();

        void load(const std::string& path);
        const std::string& play(size_t id);
    private:
        TextManager _textManager;
        SoundManager _soundManager;
    };
}