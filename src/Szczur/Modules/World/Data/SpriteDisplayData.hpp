#pragma once

#include <string>

namespace sf3d {
	class RenderTarget;
}
#include "Szczur/Utility/SFML3D/RenderStates.hpp"
#include "Szczur/Utility/SFML3D/Sprite.hpp"
#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/Drawable.hpp"

namespace rat
{

class SpriteDisplayData : public sf3d::Drawable
{
public:

	///
	SpriteDisplayData(const std::string& name);

	///
	void loadTexture();

	///
	void loadTextureWithoutSet();

	///
	void setupSprite();

	///
	const sf3d::Texture& getTexture() const;

	///
	const std::string& getName() const;

	///
	virtual void draw(sf3d::RenderTarget& target, sf3d::RenderStates states = sf3d::RenderStates()) const override;

protected:
	
	constexpr static auto _assetsFolderPath = "";
	constexpr static auto _textureFilePath = "";

private:

	std::string _name;
	sf3d::Sprite _sprite;
	sf3d::Texture _texture;

};

}
