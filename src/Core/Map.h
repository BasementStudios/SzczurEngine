#pragma once

#include <vector>
#include <cmath>

#include "Loader.h"
#include "Object.h"
#include "../Graphics/Sprite.h"

namespace rat {
	class Map {
	private:

		CoreModules& _core;
		std::vector<std::unique_ptr<Object>> _objects;
		std::vector<std::unique_ptr<Sprite>> _explosives;

	public:

		Map(CoreModules& core) :
			_core(core) {
		}

		void init() {
			for (int i = 0; i < 512; ++i) {
				push(new Object(_core));
			}
			const Texture& tex = _core.get<Loader>().getTexture(1);
			for (int i = 0; i < 16; ++i) {
				Sprite* sprite = new Sprite;
				_explosives.emplace_back(sprite);				
				sprite->set(tex, rand());
				sprite->centerOrigin();
				sprite->base().setPosition(std::floor((i%4)*64)+40, std::floor((i/4)*64)+40);
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
			for (auto& spr : _explosives) {
				_core.get<Canvas>().render(*spr.get(), 0);
			}
		}
	};
}
