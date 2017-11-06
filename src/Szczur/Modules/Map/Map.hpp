#pragma once

#include <vector>
#include <cmath>

#include <m2/Utility/Modules.hpp>

#include <Szczur/Core/AssetsLoader.hpp>
#include <Szczur/Core/Graphics/Sprite.hpp>
#include <Szczur/Modules/Map/Object.hpp>

// TODO Stritch : Zrobić prawdziwy moduł mapy, ten jest tylko testowy
namespace rat {
	class Map : public m2::utility::ModuleBase<AssetsLoader, Canvas> { using ModuleBase::ModuleBase;
	private:

		std::vector<std::unique_ptr<Object>> _objects;
		std::vector<std::unique_ptr<Sprite>> _explosives;

	public:

		void init() {
			for (int i = 0; i < 512; ++i) {
				createNewObject();
			}
			const Texture& tex = get_module<AssetsLoader>().getTexture(m2::utility::fnv1a_32("res_test/Graphics/Effects/Explosion.png"));
			for (int i = 0; i < 16; ++i) {
				Sprite* sprite = new Sprite;
				_explosives.emplace_back(sprite);
				sprite->set(tex, rand());
				sprite->centerOrigin();
				sprite->base().setPosition(std::floor((i%4)*64)+40, std::floor((i/4)*64)+40);
			}
		}

		void createNewObject() {
			_objects.emplace_back(new Object(get_module<Canvas>()));
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
				get_module<Canvas>().draw(m2::utility::fnv1a_32("GAME"), *spr);
			}
		}
	};
}
