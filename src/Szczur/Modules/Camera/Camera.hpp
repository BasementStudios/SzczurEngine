#pragma once


#include <Szczur/Utility/SFML3D/View.hpp>
#include <Szczur/Utility/Modules/Module.hpp>
#include <Szczur/Modules/Window/Window.hpp>

namespace rat {

    class Camera : public Module<Window> {
    public:
        Camera();
        void setView(const sf3d::View& view);
        void update();
    private:
        sf3d::View _view;
    };

}
