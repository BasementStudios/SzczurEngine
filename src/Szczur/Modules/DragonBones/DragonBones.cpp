#include "DragonBones.hpp"

namespace rat {
	void DragonBones::init() {
		_factory = std::make_unique<dragonBones::SFMLFactory>();
	}

	void DragonBones::update(float deltaTime) {
		_factory->update(deltaTime);
	}

	void DragonBones::render() {

	}

	void DragonBones::input(sf::Event& e) {
		(void)e;
	}
}
