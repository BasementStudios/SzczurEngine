#pragma once

#include <vector>

#include "Object.h"

namespace rat {
	class Map {
	private:

		CoreModules& _core;
		std::vector<std::unique_ptr<Object>> _objects;

	public:

		Map(CoreModules& core) :
			_core(core) {
		}

		void init() {
			for (int i = 0; i < 500; ++i) {
				push(new Object(_core));
			}
		}

		void push(Object* newObject) {
			_objects.emplace_back(newObject);
		}

		void update(float deltaTime) {
			for (auto& obj : _objects) {
				obj->update(deltaTime);
			}
		}

		void render() {
			for (auto& obj : _objects) {
				obj->render();
			}
		}
	};
}
