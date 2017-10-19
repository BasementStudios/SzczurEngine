// To jest zwyczajny ResourceManager tylko pod krótszą nazwą.
// Jest to jednak tylko placeholder tak więc nie krzyczeć
// Napisany na szybko by móc zacząć pisać klasy odpowiedzialne za grafikę

#include <iostream>
#include <vector>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "DataTypes/Texture.h"

namespace rat {
	class Loader {
	private:
		std::vector<Texture> _textures;
		
	public:
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
					std::cout<<"    > "<<path<<textureFilename<<".png frames("<<horiz<<", "<<vert<<")\n";
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
				std::cout<<"[Loading][TextureData] : "<<dataDirectoriesPath<<"\n";
				while(file>>path) {
					if(loadTexturesFromDataFile(path)) {
						std::cout<<"  [Loading][Texture] : "<<path<<"\n";
					} else {
						std::cout<<"  [Loading][Warning] : No data file in path "<<path<<"\n";
					}				
				}				
			}
		}
	};
}