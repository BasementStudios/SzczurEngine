#pragma once

#include <vector>
#include <memory>

#include <Szczur/Utility/SFML3D/Texture.hpp>

namespace rat {

class SpriteDisplayData;
class Script;

class TextureDataHolder 
{

public:

	using Data_t = std::pair<std::unique_ptr<SpriteDisplayData>,bool>;

	/// Push texture to queue
	const sf3d::Texture& getTexture(const std::string& filePath, bool reload = true);

	/// Push texture to queue
	SpriteDisplayData* getData(const std::string& filePath, bool reload = true);

	/// Load all textures from queue
	void loadAll();

	/// NEED TO BE FIX!!! Load all textures from queue in new thread
	void loadAllInNewThread();

	Data_t* find(const std::string& filePath);

	static void initScript(Script& script);

private:


	/// pairs of data and isLoaded
	std::vector<Data_t> _data;
	bool _allLoaded = true;

	int _threadStatus = 0;
};

}