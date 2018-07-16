#include "ArmatureDisplayData.hpp"

#include <experimental/filesystem>
 
#include "Szczur/Modules/DragonBones/SF3DFactory.hpp"
#include "Szczur/Utility/Logger.hpp"

#include "Szczur/Modules/FileSystem/DragDrop.hpp"

using std::chrono::system_clock;

namespace rat
{
 
ArmatureDisplayData::ArmatureDisplayData(const std::string& path)
    : _folderPath(path)
{
	_name = std::experimental::filesystem::path(path).filename().string();

	checkForReload();
	load();
	_needReload = false;
}
    
ArmatureDisplayData::~ArmatureDisplayData() 
{
	unload();
}

void ArmatureDisplayData::load()
{
	LOG_INFO("Loading armature data: '", _name, "'...");

	auto dbFactory = dragonBones::SF3DFactory::get();

	_texture = new sf3d::Texture;
	_texture->loadFromFile(_folderPath + _textureFilePath);

	dbFactory->loadDragonBonesData(_folderPath  + _skeFilePath, _name);
	dbFactory->loadTextureAtlasData(_folderPath + _textureAtlasFilePath, _texture, _name);
}

void ArmatureDisplayData::unload()
{
	LOG_INFO("Removing armature data: '", _name, "'...");

	auto dbFactory = dragonBones::SF3DFactory::get();

	dbFactory->removeDragonBonesData(_name);
	dbFactory->removeTextureAtlasData(_name);
	delete _texture;
}

void ArmatureDisplayData::reload()
{
	checkForReload();

	if (_needReload)
	{
		unload();
		load();

		_needReload = false;
	}
}

void ArmatureDisplayData::checkForReload()
{
	namespace fs = std::experimental::filesystem;

	auto lastSkeFileUpdate			= system_clock::to_time_t(fs::last_write_time(_folderPath + _skeFilePath));
	auto lastTextureAtlasFileUpdate = system_clock::to_time_t(fs::last_write_time(_folderPath + _textureAtlasFilePath));
	auto lastTextureFileUpdate		= system_clock::to_time_t(fs::last_write_time(_folderPath + _textureFilePath));

	if (lastSkeFileUpdate			!= _lastSkeFileUpdate ||
		lastTextureAtlasFileUpdate	!= _lastTextureAtlasFileUpdate ||
		lastTextureFileUpdate		!= _lastTextureFileUpdate)
	{
		_lastSkeFileUpdate = lastSkeFileUpdate;
		_lastTextureAtlasFileUpdate = lastTextureAtlasFileUpdate;
		_lastTextureFileUpdate = lastTextureFileUpdate;
		_needReload = true;
	}
}
 
}
 
