#pragma once

#include <dragonBones\DragonBonesHeaders.h>
#include <SFML\Graphics\Texture.hpp>

class WrapperTextureAtlasData : public dragonBones::TextureAtlasData
{
	BIND_CLASS_TYPE_B(WrapperTextureAtlasData);

private:
	sf::Texture* _renderTexture;

public:
	WrapperTextureAtlasData();
	~WrapperTextureAtlasData();

	virtual dragonBones::TextureData* createTexture() const override;

	sf::Texture* getRenderTexture() const { return _renderTexture; }

	void setRenderTexture(sf::Texture* value);

private:
	void clear();
};
