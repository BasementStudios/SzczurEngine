#pragma once

#include <string>
#include <vector>
#include <memory>

#include <Szczur/Utility/SFML3D/Texture.hpp>

namespace rat
{

class BattleTextureManager 
{

	struct Texture {
		std::string nameID;
		std::string path;
		sf3d::Texture texture;
	};

public:

	void loadAllTextures(const std::string& path);

	void pushNewTexture(const std::string& nameID, const std::string& path);

	sf3d::Texture* getTexture(const std::string& nameID);

private:

	std::string _graphicsPath;

	std::vector<std::unique_ptr<Texture>> _textures;

};

}