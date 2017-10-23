#pragma once

#include <vector>
#include <cmath>

#include <Szczur/Core/ModuleBase.h>
#include <Szczur/Core/AssetsLoader.h>
#include <Szczur/Core/Graphics/Sprite.h>
#include <Szczur/Game/Map/Object.h>

// TODO Stritch : Zrobić prawdziwy moduł mapy, ten jest tylko testowy
namespace rat {
	class Map : public ModuleBase<AssetsLoader, Canvas> { using ModuleBase::ModuleBase;
	private:

		std::vector<std::unique_ptr<Object>> _objects;
		std::vector<std::unique_ptr<Sprite>> _explosives;

	public:

		void init() {
			for (int i = 0; i < 512; ++i) {
				push(new Object(getModule<Canvas>()));
			}
			const Texture& tex = getModule<AssetsLoader>().getTexture(1);
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
				getModule<Canvas>().render(*spr.get(), 0);
			}
		}
	};
}
