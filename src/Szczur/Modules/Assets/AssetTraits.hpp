#pragma once

#include <string>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "Szczur/Debug/Logger.hpp"

namespace rat
{

template <typename>
struct AssetTraits;

template <>
struct AssetTraits<sf::Font>
{
	static sf::Font* create();

	static sf::Font* createFallback();

	static bool load(sf::Font& font, const std::string& path);

	static void unload(sf::Font& font);

	static const char* getName();

};

template <>
struct AssetTraits<sf::Texture>
{
	static sf::Texture* create();

	static sf::Texture* createFallback();

	static bool load(sf::Texture& texture, const std::string& path);

	static void unload(sf::Texture& texture);

	static const char* getName();

};

template <>
struct AssetTraits<sf::Shader>
{
	static sf::Shader* create();

	static sf::Shader* createFallback();

	static bool load(sf::Shader& shader, const std::string& path);

	static void unload(sf::Shader& shader);

	static const char* getName();

};

template <>
struct AssetTraits<sf::Music>
{
	static sf::Music* create();

	static sf::Music* createFallback();

	static bool load(sf::Music& music, const std::string& path);

	static void unload(sf::Music& music);

	static const char* getName();

};

template <>
struct AssetTraits<sf::SoundBuffer>
{
	static sf::SoundBuffer* create();

	static sf::SoundBuffer* createFallback();

	static bool load(sf::SoundBuffer& soundBuffer, const std::string& path);

	static void unload(sf::SoundBuffer& soundBuffer);

	static const char* getName();

};

}
