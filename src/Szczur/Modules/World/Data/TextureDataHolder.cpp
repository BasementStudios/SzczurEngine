#include "TextureDataHolder.hpp"

#include <thread>

#include "SpriteDisplayData.hpp"

#include <Szczur/Utility/Logger.hpp>

#include <Szczur/Modules/Script/Script.hpp>

namespace rat {

const sf3d::Texture& TextureDataHolder::getTexture(const std::string& filePath, bool reload) {

	// If same texture is loaded
	if(auto* data = find(filePath)) {
		if(reload) {
			data->second = false;
			_allLoaded = false;			
		}
		return data->first->getTexture();
	}

	auto& data = _data.emplace_back(new SpriteDisplayData(filePath), false);
	_allLoaded = false;

	return data.first->getTexture();
}
	
SpriteDisplayData* TextureDataHolder::getData(const std::string& filePath, bool reload) {
	
	// If same texture is loaded
	if(auto* data = find(filePath)) {
		if(reload) {
			data->second = false;
			_allLoaded = false;			
		}
		return data->first.get();
	}

	auto& data = _data.emplace_back(new SpriteDisplayData(filePath), false);
	_allLoaded = false;

	return data.first.get();
}

void TextureDataHolder::loadAll() {
	if(_allLoaded) return;
	_allLoaded = true;
	int size = _data.size();
	int i = 1;
	for(auto& obj : _data) {
		if(!obj.second) {
			obj.first->loadTexture();
			obj.second = true;
			std::cout<<"Loading textures: "<<i<<'/'<<size<<" | "<<obj.first->getName()<<std::endl;
			// LOG_INFO("Loaded: ", obj.first->getName());
		}
		++i;
	}
	std::cout<<"Textures loaded!"<<std::endl;
}

void TextureDataHolder::loadAllInNewThread() {

	if(_allLoaded) return;

	//LOG_INFO("A");
	if(_threadStatus == 0) {
		//LOG_INFO("B");
		_threadStatus = 1;
		//LOG_INFO("C");
		std::thread thread([&]{
			int size = _data.size();
			//LOG_INFO("E");
			int i = 1;
			//LOG_INFO("F");
			for(auto& obj : _data) {
				//LOG_INFO("G");
				if(!obj.second) {
					//LOG_INFO("H");
					obj.first->loadTextureWithoutSet();
					//LOG_INFO("I");
					std::cout<<"Loading textures: "<<i<<'/'<<size<<" | "<<obj.first->getName()<<std::endl;
					//LOG_INFO("J");
				}
				++i;
			}
			_threadStatus = 2;
		});
		thread.join();
		//LOG_INFO("K");
	}
	else if(_threadStatus == 2) {
		//LOG_INFO("X1");
		for(auto& obj : _data) {
			//LOG_INFO("X2");
			if(!obj.second) {
				//LOG_INFO("X3");
				obj.first->setupSprite();
				//LOG_INFO("X4");
				obj.second = true;
			}
		}
		_threadStatus = 0;
		_allLoaded = true;
	}
}

TextureDataHolder::Data_t* TextureDataHolder::find(const std::string& filePath) {
	for(auto& data : _data) {
		if(data.first->getName() == filePath) {
			return &data;
		}
	}
	return nullptr;
}

void TextureDataHolder::initScript(Script& script) {
	auto object = script.newClass<TextureDataHolder>("TextureDataHolder", "World");

	object.set("getData", &TextureDataHolder::getData);

	object.init();
}

}