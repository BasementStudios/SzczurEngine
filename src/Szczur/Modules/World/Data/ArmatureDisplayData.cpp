#include "ArmatureDisplayData.hpp"
 
#include "Szczur/Modules/DragonBones/SF3DFactory.hpp"
 
namespace rat
{
 
    ArmatureDisplayData::ArmatureDisplayData(const std::string& name)
        : _name(name)
    {
        auto dbFactory = dragonBones::SF3DFactory::get();

        _texture = new sf3d::Texture;
        _texture->loadFromFile(_assetsFolderPath + _name + _textureFilePath);

        dbFactory->loadDragonBonesData(_assetsFolderPath + _name + _skeFilePath);
        dbFactory->loadTextureAtlasData(_assetsFolderPath + _name + _textureAtlasFilePath, _texture);
    }
    
    ArmatureDisplayData::~ArmatureDisplayData() {
        auto dbFactory = dragonBones::SF3DFactory::get();

        dbFactory->removeDragonBonesData(_name);
        dbFactory->removeTextureAtlasData(_name);
        delete _texture;
    }
 
}
 