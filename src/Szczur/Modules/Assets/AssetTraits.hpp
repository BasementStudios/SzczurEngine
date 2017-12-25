#pragma once

#include <string>

namespace sf
{

class Font;
class Texture;
class Shader;
class Music;
class SoundBuffer;

}

namespace rat
{

template<typename T>
struct AssetTraits;

template<>
struct AssetTraits<sf::Font>
{
	static sf::Font* create();

	static bool load(sf::Font& font, const std::string& path);

	static void unload(sf::Font& font);

};

template<>
struct AssetTraits<sf::Texture>
{
	static sf::Texture* create();

	static bool load(sf::Texture& texture, const std::string& path);

	static void unload(sf::Texture& texture);

};

template<>
struct AssetTraits<sf::Shader>
{
	static sf::Shader* create();

	static bool load(sf::Shader& shader, const std::string& path);

	static void unload(sf::Shader& shader);

};

template<>
struct AssetTraits<sf::Music>
{
	static sf::Music* create();

	static bool load(sf::Music& music, const std::string& path);

	static void unload(sf::Music& music);

};

template<>
struct AssetTraits<sf::SoundBuffer>
{
	static sf::SoundBuffer* create();

	static bool load(sf::SoundBuffer& soundBuffer, const std::string& path);

	static void unload(sf::SoundBuffer& soundBuffer);

};

}
