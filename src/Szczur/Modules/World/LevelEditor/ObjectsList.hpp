#pragma once

namespace rat {
    class ScenesManager;
    class ObjectsList {
    public:
        ObjectsList(ScenesManager& scenes);
        void render();
        void update();

        bool& getBool();
    private:
        ScenesManager& _scenes;

        bool _ifRenderObjectsList{true};
    };
}