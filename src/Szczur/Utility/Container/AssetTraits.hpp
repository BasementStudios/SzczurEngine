#pragma once

#include <memory>

#include <SFML/Graphics/Texture.hpp>

namespace rat
{

template <typename T>
struct AssetTraits;

template <>
struct AssetTraits<sf::Texture>
{

	///
	static sf::Texture* create()
	{
		return new sf::Texture{};
	}

	///
	static bool loadFromFile(sf::Texture& texture, const std::string& path)
	{
		return texture.loadFromFile(path);
	}

};

}
