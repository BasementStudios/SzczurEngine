#pragma once
 
#include <ctime>

#include "Szczur/Utility/SFML3D/Drawable.hpp"
 
#include "Szczur/Modules/DragonBones/SF3DArmatureDisplay.hpp"
 
namespace rat
{ 

class ArmatureDisplayData
{
private:
    sf3d::Texture* _texture;

    std::string _name;
    std::string _folderPath;

	bool _needReload = false;
	std::time_t _lastSkeFileUpdate = 0;
	std::time_t _lastTextureAtlasFileUpdate = 0;
	std::time_t _lastTextureFileUpdate = 0;

protected:
    constexpr static auto _assetsFolderPath = "";
    constexpr static auto _skeFilePath = "/armature_ske.json";
    constexpr static auto _textureAtlasFilePath = "/armature_tex.json";
    constexpr static auto _textureFilePath = "/armature_tex.png";

public:
    ArmatureDisplayData(const std::string& path);
    ~ArmatureDisplayData();

	void load();
	void unload();
	void reload();

	void checkForReload();

    const auto& getName() { return _name; }
    const auto& getFolderPath() { return _folderPath; }
};
 
}
