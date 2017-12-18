#include "AssetTraits.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace rat
{

template<>
struct AssetTraits<sf::Font>
{
	static sf::Font* create()
	{
		return new sf::Font;
	}

	static bool load(sf::Font& font, const std::string& path)
	{
		return font.loadFromFile(path);
	}

	static void unload(sf::Font&)
	{

	}

};

template<>
struct AssetTraits<sf::Texture>
{
	static sf::Texture* create()
	{
		return new sf::Texture;
	}

	static bool load(sf::Texture& texture, const std::string& path)
	{
		return texture.loadFromFile(path);
	}

	static void unload(sf::Texture&)
	{

	}

};

template<>
struct AssetTraits<sf::Music>
{
	static sf::Music* create()
	{
		return new sf::Music;
	}

	static bool load(sf::Music& music, const std::string& path)
	{
		return music.openFromFile(path);
	}

	static void unload(sf::Music&)
	{

	}

};

template<>
struct AssetTraits<sf::SoundBuffer>
{
	static sf::SoundBuffer* create()
	{
		return new sf::SoundBuffer;
	}

	static bool load(sf::SoundBuffer& soundBuffer, const std::string& path)
	{
		return soundBuffer.loadFromFile(path);
	}

	static void unload(sf::SoundBuffer&)
	{

	}

};

}
