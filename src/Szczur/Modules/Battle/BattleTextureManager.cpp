#include "BattleTextureManager.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

#include <Szczur/Utility/Logger.hpp>

namespace rat
{

void BattleTextureManager::loadAllTextures(const std::string& path)
{

	std::ifstream file(path);
	if(file.good()) {
		_graphicsPath = path;

		nlohmann::json config;
		file>>config;

		if(auto& pawns = config["textures"]; !pawns.is_null()) {

			_textures.clear();

			for(auto itr = pawns.begin(); itr != pawns.end(); ++itr) {

				try {
					pushNewTexture(itr.key(), itr.value().get<std::string>());
				}
				catch(nlohmann::json::exception e) {
					LOG_EXCEPTION(e);
				}
			}
		}
	}
	else {
		LOG_ERROR("Cannot load battle textures: ", path);
	}
}

void BattleTextureManager::pushNewTexture(const std::string& nameID, const std::string& path)
{
	for(auto& tex : _textures) {
		if(tex->nameID == nameID) {
			return;
		}
	}

	auto* newTex = _textures.emplace_back(new BattleTextureManager::Texture()).get();
	newTex->nameID = nameID;
	newTex->path = path;
	try {
		newTex->texture.loadFromFile(path);
	}
	catch (const std::exception& exc){
		_textures.pop_back();
		LOG_EXCEPTION(exc);
	}	
	
}

sf3d::Texture* BattleTextureManager::getTexture(const std::string& nameID)
{
	for(auto& tex : _textures) {
		if(tex->nameID == nameID) {
			return &tex->texture;
		}
	}
	return nullptr;
}

}