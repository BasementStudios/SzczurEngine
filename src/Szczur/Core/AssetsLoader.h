#pragma once

#include <iostream>
#include <vector>
#include <fstream>

#include <SFML/Graphics.hpp>

#include <Szczur/Core/ModuleBase.h>
#include <Szczur/Core/Graphics/Texture.h>

namespace rat {
	class AssetsLoader : public ModuleBase<> { using ModuleBase::ModuleBase;
	private:

		std::vector<Texture> _textures;

	public:

		void init() {
			
		}

	// Searching
		// TODO : zrobić wyszukiwanie z hashowaniem
		const Texture& getTexture(int index) {
			return _textures[index];
		}

	// Modifiers
		void loadNewTexture(const std::string& filename, int horiz, int vert) {
			_textures.emplace_back(filename, horiz, vert);
		}
		void loadNewTexture(const std::string& filename, const sf::Vector2i& frames) {
			_textures.emplace_back(filename, frames);
		}
		// Load all textures in one files directory
		bool loadTexturesFromDataFile(const std::string& dataPath) {
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
		// loadTexturesFromDataFile for all directories in dataFile
		void loadTexturesFromDataDirectories(const std::string& dataDirectoriesPath) {
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
	};
}
