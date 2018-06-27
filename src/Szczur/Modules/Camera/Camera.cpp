#include "Camera.hpp"

namespace rat {
    Camera::Camera() :
    _view(getModule<Window>().getWindow().getDefaultView()) {
        _view.move({0.f, 0.f, 1000.f});
    }

    void Camera::setView(const sf3d::View& view) {
        _view.setCenter(view.getCenter());
        _view.setRotation(view.getRotation());
    }
    
    const sf3d::View& Camera::getView() const {
        return _view;
    }

    void Camera::update() {

        getModule<Window>().getWindow().setView(_view);
    }


}