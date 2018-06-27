#include "TextureDataHolder.hpp"

#include <thread>
#include <experimental/filesystem>


#include "SpriteDisplayData.hpp"

#include <Szczur/Utility/Logger.hpp>

#include <Szczur/Modules/Script/Script.hpp>

namespace rat {

TextureDataHolder::TextureData::TextureData(SpriteDisplayData* data)
	: data(data), reloaded(false) {
	lastWriten = std::experimental::filesystem::last_write_time(data->getName());	
}

bool TextureDataHolder::TextureData::checkTime() {
	return lastWriten == std::experimental::filesystem::last_write_time(data->getName());	
}

void TextureDataHolder::TextureData::updateTime() {
	lastWriten = std::experimental::filesystem::last_write_time(data->getName());	
}

const sf3d::Texture& TextureDataHolder::getTexture(const std::string& filePath, bool reload) {

	// If same texture is loaded
	if(auto* data = find(filePath)) {
		if(reload && !data->checkTime()) {
			data->reloaded = false;
			_allLoaded = false;			
		}
		return data->data->getTexture();
	}

	auto& data = _data.emplace_back(new SpriteDisplayData(filePath));
	_allLoaded = false;

	return data.data->getTexture();
}
	
SpriteDisplayData* TextureDataHolder::getData(const std::string& filePath, bool reload) {
	
	// If same texture is loaded
	if(auto* data = find(filePath)) {
		if(reload && !data->checkTime()) {
			data->reloaded = false;
			_allLoaded = false;
		}
		return data->data.get();
	}

	auto& data = _data.emplace_back(new SpriteDisplayData(filePath));
	_allLoaded = false;

	return data.data.get();
}

void TextureDataHolder::loadAll() {
	if(_allLoaded) return;
	_allLoaded = true;
	int size = _data.size();
	int i = 1;
	for(auto& obj : _data) {
		if(!obj.reloaded) {
			obj.data->loadTexture();
			obj.reloaded = true;
			obj.updateTime();
			std::cout<<"Loading textures: "<<i<<'/'<<size<<" | "<<obj.data->getName()<<std::endl;
			// LOG_INFO("Loaded: ", obj.data->getName());
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
				if(!obj.reloaded) {
					//LOG_INFO("H");
					obj.data->loadTextureWithoutSet();
					//LOG_INFO("I");
					std::cout<<"Loading textures: "<<i<<'/'<<size<<" | "<<obj.data->getName()<<std::endl;
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
			if(!obj.reloaded) {
				//LOG_INFO("X3");
				obj.data->setupSprite();
				//LOG_INFO("X4");
				obj.reloaded = true;
			}
		}
		_threadStatus = 0;
		_allLoaded = true;
	}
}

TextureDataHolder::TextureData* TextureDataHolder::find(const std::string& filePath) {
	for(auto& data : _data) {
		if(data.data->getName() == filePath) {
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