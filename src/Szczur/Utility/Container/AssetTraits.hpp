#pragma once

#include <string>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

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

template <>
struct AssetTraits<sf::Font>
{
	///
	static sf::Font* create()
	{
		return new sf::Font{};
	}

	///
	static bool loadFromFile(sf::Font& texture, const std::string& path)
	{
		return texture.loadFromFile(path);
	}

};

}
