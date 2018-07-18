#include "TextureDataHolder.hpp"

#include <thread>
#include <experimental/filesystem>


#include "SpriteDisplayData.hpp"

#include <Szczur/Utility/Logger.hpp>

#include <Szczur/Modules/Script/Script.hpp>

namespace rat {

TextureDataHolder::TextureData::TextureData(SpriteDisplayData* data)
	: data(data), reloaded(false) {
#ifndef PSYCHOX
	lastWriten = std::experimental::filesystem::last_write_time(data->getName());	
#endif
}

bool TextureDataHolder::TextureData::checkTime() {
#ifndef PSYCHOX
	return lastWriten == std::experimental::filesystem::last_write_time(data->getName());	
#else
	return false;
#endif
}

void TextureDataHolder::TextureData::updateTime() {
#ifndef PSYCHOX
	lastWriten = std::experimental::filesystem::last_write_time(data->getName());
#endif
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

	if (_allLoaded) return;
	
	if (_threadStatus == 0) {
		_threadStatus = 1;
		std::thread thread([&]{
			int size = _data.size();
			int i = 1;
			for (auto& obj : _data) {
				if (!obj.reloaded) {
					obj.data->loadTextureWithoutSet();
					std::cout << "Loading textures: " << i << '/' << size << " | " << obj.data->getName() << std::endl;
				}
				++i;
			}
			_threadStatus = 2;
		});
		thread.join();
	}
	else if (_threadStatus == 2) {
		for (auto& obj : _data) {
			if (!obj.reloaded) {
				obj.data->setupSprite();
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