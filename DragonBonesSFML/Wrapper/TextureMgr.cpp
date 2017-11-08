#include "TextureMgr.h"

#include <string>

#include <SFML\Graphics.hpp>

#include "Common.h"

TextureMgr *TextureMgr::s_pInst;

TextureMgr::TextureMgr()
{
	s_pInst = this;
}


TextureMgr::~TextureMgr()
{
}

sf::Texture* TextureMgr::GetTexture(const std::string &imagePath)
{
	/*for (auto& tex : m_textures)
	{
		if (tex && tex->m_imagePath == imagePath)
		{
			return tex->texture.get();
		}
	}*/

	printf("Loading texture: '%s'... ", imagePath.c_str());

	if (isFileExist(imagePath.c_str()))
	{
		auto texture = std::make_shared<sf::Texture>();
		//texture->texture = std::make_shared<sf::Texture>();

		if (texture->loadFromFile(imagePath))
		{
			//texture = imagePath;

			m_textures.push_back(texture);

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
