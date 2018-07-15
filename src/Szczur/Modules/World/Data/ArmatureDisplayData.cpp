#include "ArmatureDisplayData.hpp"

#include <experimental/filesystem>
 
#include "Szczur/Modules/DragonBones/SF3DFactory.hpp"
#include "Szczur/Utility/Logger.hpp"

namespace rat
{
 
ArmatureDisplayData::ArmatureDisplayData(const std::string& path)
    : _folderPath(path)
{
    _name = std::experimental::filesystem::path(path).filename().string();

    auto dbFactory = dragonBones::SF3DFactory::get();

    _texture = new sf3d::Texture;
    _texture->loadFromFile(_folderPath + _textureFilePath);

    dbFactory->loadDragonBonesData(_folderPath + _skeFilePath, _name);
    dbFactory->loadTextureAtlasData(_folderPath + _textureAtlasFilePath, _texture, _name);

	// LOG_INFO("Added: ", _name);
}
    
ArmatureDisplayData::~ArmatureDisplayData() 
{
    auto dbFactory = dragonBones::SF3DFactory::get();

	dbFactory->removeDragonBonesData(_name);
	dbFactory->removeTextureAtlasData(_name);
	delete _texture;

	// LOG_INFO("Removed: ", _name);
}
 
}
 
