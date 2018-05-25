#pragma once

namespace rat {
    class ScenesManager;
    class ObjectParameters {
    public:
        ObjectParameters(ScenesManager& scenes);
        void render();
    private:
        ScenesManager& _scenes;

        void _renderComponentsManager();
    };
}