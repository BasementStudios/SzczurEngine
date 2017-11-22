#include "TextureMgr.h"

#include <string>

#include <SFML\Graphics.hpp>

#include "Common.h"

TextureMgr *TextureMgr::_textureMgr;

TextureMgr::TextureMgr()
{
	_textureMgr = this;
}

TextureMgr::~TextureMgr()
{
}

sf::Texture* TextureMgr::GetTexture(const std::string &imagePath)
{
	if (_textures.find(imagePath) != _textures.end())
	{
		return _textures[imagePath].get();
	}

	printf("Loading texture: '%s'... ", imagePath.c_str());

	if (isFileExist(imagePath.c_str()))
	{
		auto texture = std::make_shared<sf::Texture>();

		if (texture->loadFromFile(imagePath))
		{
			_textures[imagePath] = texture;

			printf("[OK]\n");

			return texture.get();
		}
		else
		{
			printf("[ERROR]\n");
			return nullptr;
		}
	}
	else
	{
		printf("[FILE MISSING]\n");
		return nullptr;
	}
}
