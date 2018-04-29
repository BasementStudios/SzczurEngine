#include "Camera.hpp"

namespace rat {
    Camera::Camera() :
    _view(getModule<Window>().getWindow().getDefaultView()) {
        _view.rotate({20.f, 0.f, 0.f});
        _view.move({100.f, 0.f, 0.f});
    }

    void Camera::setView(const sf3d::View& view) {
        _view.setCenter(view.getCenter());
        _view.setRotation(view.getRotation());
    }

    void Camera::update() {

        getModule<Window>().getWindow().setView(_view);
    }


}