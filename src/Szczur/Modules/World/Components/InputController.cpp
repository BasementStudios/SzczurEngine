#include "InputController.hpp"

namespace rat {
    InputController::InputController() {

    }

    void InputController::update(const InputManager& input) {
        if(input.isKept(Keyboard::W))
            move({0.f, 0.f, -10.f});
        if(input.isKept(Keyboard::A))
            move({-10.f, 0.f, 0.f});
        if(input.isKept(Keyboard::S))
            move({-10.f, 0.f, 10.f});
        if(input.isKept(Keyboard::D))
            move({10.f, 0.f, 0.f});
    }
}