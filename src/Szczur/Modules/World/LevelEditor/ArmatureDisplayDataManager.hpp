#pragma once

namespace rat {
    class ScenesManager;
    class ArmatureDisplayDataManager {
    public:
        ArmatureDisplayDataManager(ScenesManager& scenes);
        void render();
        bool& getBool();
    private:
        ScenesManager& _scenes;

        bool _ifRender{false};
    };
}