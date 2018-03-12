
Armature* DragonBones::createArmature(const std::string& actorName)
{
	std::string path = actorName.data();

	auto dbData = _factory.loadDragonBonesData(path + "/skeleton.json");

	if (!dbData)
		return nullptr;

	auto texturesData = _factory.getTexturesData(dbData, path + "/textures");

	for (auto& textureData : texturesData) {
		auto tex = &getModule<Assets>().load<sf::Texture>(textureData->path);
		textureData->setTexture(tex);
	}

	auto textureAtlasData = _factory.createTextureAtlasData(texturesData, dbData);

	if (textureAtlasData == nullptr) {
		for (auto& textureData : texturesData) {
			if (textureData)
				delete textureData;
		}

		return nullptr;
	}

	auto armatureDisplay = _factory.buildArmatureDisplay(actorName.data());

	if (!armatureDisplay)
		return nullptr;

	auto armature = new Armature(armatureDisplay);

	return armature;
}