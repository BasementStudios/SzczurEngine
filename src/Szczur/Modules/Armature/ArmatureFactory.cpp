#include "ArmatureFactory.hpp"

void rat::ArmatureFactory::init() {
    _factory = std::make_unique<dragonBones::SFMLFactory>();
}

void rat::ArmatureFactory::update(float deltaTime) {
    _factory->update(deltaTime);
}

