#pragma once

namespace rat {
    class ScenesManager;
    class SpriteDisplayDataManager {
    public:
        SpriteDisplayDataManager(ScenesManager& scenes);
        void render();
        bool& getBool();
    private:
        ScenesManager& _scenes;

        bool _ifRender{false};
    };
}