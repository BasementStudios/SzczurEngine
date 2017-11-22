#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include <SFML\Graphics.hpp>

class TextureMgr
{
private:
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> _textures;

	static TextureMgr *_textureMgr;

public:
	TextureMgr();
	~TextureMgr();

	sf::Texture *GetTexture(const std::string &path);

	static TextureMgr *Get() { return _textureMgr; }
};

