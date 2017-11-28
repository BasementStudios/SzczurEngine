#include "DragonBones.hpp"

void rat::DragonBones::init() {
    _factory = std::make_unique<dragonBones::SFMLFactory>();
}

void rat::DragonBones::update(float deltaTime) {
    _factory->update(deltaTime);
}
