#pragma once

#include <fstream>
#include <streambuf>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/MemoryInputStream.hpp>

namespace rat
{

template<typename T>
struct AssetTraits;

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

	static void unload(sf::Font& font)
	{
		font.~Font();
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

	static void unload(sf::Texture& texture)
	{
		texture.~Texture();
	}

};

template<>
struct AssetTraits<sf::Shader>
{
	static sf::Shader* create()
	{
		return new sf::Shader;
	}

	static bool load(sf::Shader& shader, const std::string& path)
	{
		std::ifstream file(path);

		if(!file.good())
			return false;

		std::string buffer;

		file.seekg(0, std::ios::end);
		buffer.reserve(file.tellg());
		file.seekg(0, std::ios::beg);

		buffer.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

		sf::MemoryInputStream streams[3];

		auto it1 = std::begin(buffer);
		auto it2 = std::find(it1, std::end(buffer), '$');
		streams[0].open(it1.base(), std::distance(it1, it2));

		it1 = std::next(it2);
		it2 = std::find(it1, std::end(buffer), '$');
		streams[1].open(it1.base(), std::distance(it1, it2));

		it1 = std::next(it2);
		it2 = std::find(it1, std::end(buffer), '$');
		streams[2].open(it1.base(), std::distance(it1, it2));

		return shader.loadFromStream(streams[0], streams[1], streams[2]);
	}

	static void unload(sf::Shader& shader)
	{
		shader.~Shader();
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

	static void unload(sf::Music& music)
	{
		music.~Music();
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

	static void unload(sf::SoundBuffer& soundBuffer)
	{
		soundBuffer.~SoundBuffer();
	}

};

}
