#pragma once

#include <vector>
#include <string>
#include <memory>

namespace sf
{
	class Texture;
}

class TextureMgr
{
public:
	class TextureInfo
	{
	public:
		std::shared_ptr<sf::Texture>	texture;
		std::string						m_imagePath;
	};

private:
	std::vector<std::shared_ptr<sf::Texture>>	m_textures;

	static TextureMgr							*s_pInst;

public:
	TextureMgr();
	~TextureMgr();

	sf::Texture *GetTexture(const std::string &path);

	static TextureMgr *Get() { return s_pInst; }
};

