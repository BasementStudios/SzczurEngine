#pragma once

#include <vector>
#include <memory>
#include <experimental/filesystem>

#include <Szczur/Utility/SFML3D/Texture.hpp>

namespace rat {

class SpriteDisplayData;
class Script;

class TextureDataHolder 
{

	struct TextureData {
		std::unique_ptr<SpriteDisplayData> data;
		bool reloaded = false;
#ifndef PSYCHOX
		std::experimental::filesystem::file_time_type lastWriten;
#endif

		///
		TextureData(SpriteDisplayData* data);

		///
		bool checkTime();

		///
		void updateTime();
	};

public:

	/// Push texture to queue
	const sf3d::Texture& getTexture(const std::string& filePath, bool reload = true);

	/// Push texture to queue
	SpriteDisplayData* getData(const std::string& filePath, bool reload = true);

	/// Load all textures from queue
	void loadAll();

	/// NEED TO BE FIX!!! Load all textures from queue in new thread
	void loadAllInNewThread();

	TextureData* find(const std::string& filePath);

	static void initScript(Script& script);

private:


	/// pairs of data and isLoaded
	std::vector<TextureData> _data;
	bool _allLoaded = true;

	int _threadStatus = 0;
};

}