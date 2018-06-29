#pragma once
 
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
    
protected:
    constexpr static auto _assetsFolderPath = "";
    constexpr static auto _skeFilePath = "/armature_ske.json";
    constexpr static auto _textureAtlasFilePath = "/armature_tex.json";
    constexpr static auto _textureFilePath = "/armature_tex.png";

public:
    ArmatureDisplayData(const std::string& path);
    ~ArmatureDisplayData();

    const auto& getName() { return _name; }
    const auto& getFolderPath() { return _folderPath; }
};
 
}
