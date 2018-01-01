#pragma once

/** @file AssetTraits.cpp
 ** @description Header file with standard assets traits.
 ** @author Patryk (Stritch)
 **/

#include <string>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace rat
{

template<typename>
struct AssetTraits;

template<>
struct AssetTraits<sf::Font>
{
	static constexpr const char* string = "sf::Font";
	static sf::Font* create();
	static sf::Font* load(sf::Font& font, const std::string& path);
	static void unload(sf::Font& font);
};

template<>
struct AssetTraits<sf::Texture>
{
	static constexpr const char* string = "sf::Texture";
	static sf::Texture* create();
	static sf::Texture* load(sf::Texture& texture, const std::string& path);
	static void unload(sf::Texture& texture);
};

template<>
struct AssetTraits<sf::Shader>
{
	static constexpr const char* string = "sf::Shader";
	static sf::Shader* create();
	static sf::Shader* load(sf::Shader& shader, const std::string& path);
	static void unload(sf::Shader& shader);
};

template<>
struct AssetTraits<sf::Music>
{
	static constexpr const char* string = "sf::Music";
	static sf::Music* create();
	static sf::Music* load(sf::Music& music, const std::string& path);
	static void unload(sf::Music& music);
};

template<>
struct AssetTraits<sf::SoundBuffer>
{
	static constexpr const char* string = "sf::SoundBuffer";
	static sf::SoundBuffer* create();
	static sf::SoundBuffer* load(sf::SoundBuffer& soundBuffer, const std::string& path);
	static void unload(sf::SoundBuffer& soundBuffer);
};

}
