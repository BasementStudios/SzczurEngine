#pragma once

namespace rat {
    class ScenesManager;
    class SpriteDisplayDataManager {
    public:
        SpriteDisplayDataManager(ScenesManager& scenes);
        void render(bool& ifRender);
    private:
        ScenesManager& _scenes;
    };
}