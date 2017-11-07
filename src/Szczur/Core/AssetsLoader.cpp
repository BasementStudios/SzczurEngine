#include "AssetsLoader.hpp"

#include <iostream>

namespace rat {
	void AssetsLoader::init() {

	}

	Texture& AssetsLoader::getTexture(Hash32_t textureId) {
		return *_textures.at(textureId);
	}

	const Texture& AssetsLoader::getTexture(Hash32_t textureId) const {
		return *_textures.at(textureId);
	}

	void AssetsLoader::loadNewTexture(const std::string& filename, int horiz, int vert) {
		_textures.emplace(rat::fnv1a_32(filename.data()), new rat::Texture(filename, horiz, vert));
	}

	void AssetsLoader::loadNewTexture(const std::string& filename, const sf::Vector2i& frames) {
		_textures.emplace(rat::fnv1a_32(filename.data()), new rat::Texture(filename, frames));
	}

	bool AssetsLoader::loadTexturesFromDataFile(const std::string& dataPath) {
		std::ifstream file(dataPath);
		if(file.good()) {
			std::string path(dataPath.substr(0, dataPath.find_last_of("/\\") + 1)); // for "A/B/C/File.txt" path is "A/B/C/"

			std::string textureFilename;
			int horiz, vert;
			while(file>>textureFilename>>horiz>>vert) {
				loadNewTexture(path + textureFilename + ".png", horiz, vert);
				std::cout<<"    > "<<textureFilename<<".png frames("<<horiz<<", "<<vert<<")\n";
			}
			file.close();
			return true;
		}

		return false;
	}

	void AssetsLoader::loadTexturesFromDataDirectories(const std::string& dataDirectoriesPath) {
		std::ifstream file(dataDirectoriesPath);
		std::string path;
		if(file.good()) {
			std::cout<<"[Loading textures...]\n";
			std::cout<<"Textures data : "<<dataDirectoriesPath<<'\n';
			int dataNumber = 1;
			while(file>>path) {
				if(loadTexturesFromDataFile(path)) {
					std::cout<<"  Data "<<dataNumber<<" : "<<path.substr(0, path.find_last_of("\\/"))<<"\n";
				} else {
					std::cout<<"  Warning! No data file in path "<<path<<"\n";
				}
				++dataNumber;
			}
		}
	}
}
