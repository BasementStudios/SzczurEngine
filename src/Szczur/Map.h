#pragma once

#include <vector>

#include "Core.h"
#include "Object.h"

namespace rat {
	class Map {
	private:
		Core& core;
		std::vector<Object*> objects;
	
	public:
		Map(Core& _core) :
			core(_core) {
		}
		void init() {			
			for(int i=0; i<500; ++i) {
				push(new Object(core));
			}
		}
		void push(Object* newObject) {
			objects.push_back(newObject);
		}
		void update(float deltaTime) {
			for(auto& obj : objects) {
				obj->update(deltaTime);
			}
		}
		void render() {
			for(auto& obj : objects) {
				obj->render();
			}
		}
		~Map() {
			for(auto& obj : objects) {
				delete obj;
			}
		}
	};
}